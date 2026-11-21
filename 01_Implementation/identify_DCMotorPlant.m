%IDENTIFY_DCMOTORPLANT Identify duty [%] -> speed [rpm] plant from M1 open-loop data.
%   Estimation: M1 run2 (calibrated, Enc_Kcal = 0.8284)
%   Validation: M1 run1 (uncalibrated, rescaled by Enc_Kcal)
%   Output signal is rpm_raw (before the measurement low-pass filter); the
%   filter is modelled separately in the M2 simulation model.
%   Saves 01_Implementation/DCMotorPlant_ID.mat and 02_SupportingArtifacts/M2_PlantID.png.

prj  = currentProject;
impl = fullfile(prj.RootFolder, "01_Implementation");
supp = fullfile(prj.RootFolder, "02_SupportingArtifacts");
DCMotorParams;

%% Data
d2 = load(fullfile(impl, "M1_OpenLoop_run2_data.mat"), "tt");
d1 = load(fullfile(impl, "M1_OpenLoop_run1_data.mat"), "tt");
ze = iddata(d2.tt.rpm_raw, d2.tt.duty_cmd, Ts, ...
    "InputName", "duty", "InputUnit", "%", "OutputName", "rpm", "OutputUnit", "rpm");
zv = iddata(d1.tt.rpm_raw*Enc_Kcal, d1.tt.duty_cmd, Ts, ...
    "InputName", "duty", "InputUnit", "%", "OutputName", "rpm", "OutputUnit", "rpm");
ze.InterSample = {'zoh'};  zv.InterSample = {'zoh'};   % PWM duty is piecewise constant

%% Delay first
nk = delayest(ze);
fprintf("delayest: nk = %d samples (%.3f s)\n", nk, nk*Ts);

%% Process models (step data -> procest)
opt = procestOptions("Focus", "simulation");
mP1  = procest(ze, "P1",  opt);
mP1D = procest(ze, "P1D", opt);
mP2D = procest(ze, "P2D", opt);
[~, fitE] = compare(ze, mP1, mP1D, mP2D);
[~, fitV] = compare(zv, mP1, mP1D, mP2D);
fprintf("Estimation fit  P1 / P1D / P2D: %.1f / %.1f / %.1f %%\n", fitE{:});
fprintf("Validation fit  P1 / P1D / P2D: %.1f / %.1f / %.1f %%\n", fitV{:});

% Pick P1D unless P2D is clearly better (> 2 % on validation)
if fitV{3} > fitV{2} + 2
    plantModel = mP2D;  plantName = "P2D";
else
    plantModel = mP1D;  plantName = "P1D";
end

%% Residuals on validation data
[~, r] = resid(zv, plantModel);
acf  = r(:,1,1)/r(1,1,1);
conf = 2.58/sqrt(size(zv.y,1));
fprintf("Residual autocorr within 99%% bound: %s\n", string(all(abs(acf(2:end)) < conf)));

%% Asymmetry check: separate positive and negative step fits
tE = (0:size(ze.y,1)-1)'*Ts;
zePos = ze(tE < 8.9);  zeNeg = ze(tE >= 8.9);
mPos = procest(zePos, "P1D", opt);  mNeg = procest(zeNeg, "P1D", opt);

%% Results
Plant_K   = plantModel.Kp;
Plant_Tau = plantModel.Tp1;
Plant_Td  = plantModel.Td;
if plantName == "P2D", Plant_Tau2 = plantModel.Tp2; else, Plant_Tau2 = 0; end
plantTF = tf(Plant_K, conv([Plant_Tau 1], [Plant_Tau2 1]), "InputDelay", Plant_Td);

fprintf("\n================================================================\n");
fprintf("  Plant identification  (duty %% -> rpm)\n");
fprintf("================================================================\n");
fprintf("  Model:  %s   K = %.3f rpm/%%,  Tau = %.4f s,  Tau2 = %.4f s,  Td = %.4f s\n", ...
    plantName, Plant_K, Plant_Tau, Plant_Tau2, Plant_Td);
fprintf("  Validation fit (run1, simulation): %.1f %%\n", fitV{plantName == ["P1","P1D","P2D"]});
fprintf("  +step P1D: K = %.3f, Tau = %.4f, Td = %.4f\n", mPos.Kp, mPos.Tp1, mPos.Td);
fprintf("  -step P1D: K = %.3f, Tau = %.4f, Td = %.4f\n", mNeg.Kp, mNeg.Tp1, mNeg.Td);
fprintf("  Bandwidth: %.1f rad/s\n", bandwidth(plantTF));
fprintf("================================================================\n");

save(fullfile(impl, "DCMotorPlant_ID.mat"), "plantModel", "plantName", "plantTF", ...
    "Plant_K", "Plant_Tau", "Plant_Tau2", "Plant_Td", "mP1", "mP1D", "mP2D", "mPos", "mNeg", "fitE", "fitV");

%% Plot
fig = figure("Name", "M2 plant ID", "Color", "w", "Position", [100 100 900 650]);
tl = tiledlayout(fig, 2, 1, "TileSpacing", "compact");
nexttile; yE = sim(plantModel, ze);
plot(tE, ze.y, "Color", [0.6 0.6 0.6]); hold on; plot(tE, yE.y, "LineWidth", 1.3); grid on;
ylabel("Speed [rpm]"); title(sprintf("Estimation (M1 run2) – %s fit %.1f %%", plantName, fitE{plantName == ["P1","P1D","P2D"]}));
legend("measured rpm\_raw", "model", "Location", "best");
nexttile; yV = sim(plantModel, zv); tV = (0:size(zv.y,1)-1)'*Ts;
plot(tV, zv.y, "Color", [0.6 0.6 0.6]); hold on; plot(tV, yV.y, "LineWidth", 1.3); grid on;
ylabel("Speed [rpm]"); xlabel("Time [s]");
title(sprintf("Validation (M1 run1, rescaled) – %s fit %.1f %%", plantName, fitV{plantName == ["P1","P1D","P2D"]}));
exportgraphics(tl, fullfile(supp, "M2_PlantID.png"), "Resolution", 120);
