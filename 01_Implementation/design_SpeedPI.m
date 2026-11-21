%DESIGN_SPEEDPI Tune the speed PI controller and verify it on the M2 staircase.
%   1. Linear design with pidtune on the loop  L = C * Plant_d * H, where
%      H = z^-1 * RPM_LPF (encoder delta-count delay + measurement filter).
%   2. Candidate crossover frequencies are simulated in DCMotor_M2_PISim
%      (staircase +1500 -> -1500 rpm, -150 rpm / 2 s, PWM and encoder
%      quantisation, +/-100 % saturation with clamping anti-windup).
%   3. Selection: overshoot < 10 %, settling (+/-5 % of step) < 0.5 s on all
%      150 rpm steps, PM >= 60 deg, GM >= 10 dB -> most robust (lowest wc)
%      candidate that settles in < 0.3 s, otherwise the fastest passing one.
%   Saves 01_Implementation/SpeedPI_design.mat and
%   02_SupportingArtifacts/M2_PISim_staircase.png, M2_PISim_steps.png.

prj  = currentProject;
impl = fullfile(prj.RootFolder, "01_Implementation");
supp = fullfile(prj.RootFolder, "02_SupportingArtifacts");
mdl  = "DCMotor_M2_PISim";
DCMotorParams;

%% Linear loop model
z = tf('z', Ts);
G = Plant_d;
H = z^-1 * tf(RPM_LPF_Num, RPM_LPF_Den, Ts);

wcList = [8 10 12 15 18 22];
optPID = pidtuneOptions("PhaseMargin", 65, "DesignFocus", "reference-tracking");
nC = numel(wcList);
res = table('Size', [nC 11], ...
    'VariableTypes', repmat("double", 1, 11), ...
    'VariableNames', ["wc","Kp","Ki","PM","GM_dB","OS_pct","Ts_max","Ts_mean","Ess_rms","OS_first_pct","Pass"]);
C = cell(nC, 1);
for k = 1:nC
    C{k} = pidtune(G*H, "PI", wcList(k), optPID);   % ForwardEuler, matches the block
    [Gm, Pm] = margin(C{k}*G*H);
    res.wc(k) = wcList(k);  res.Kp(k) = C{k}.Kp;  res.Ki(k) = C{k}.Ki;
    res.PM(k) = Pm;         res.GM_dB(k) = 20*log10(Gm);
end

%% Simulink staircase verification (fast restart sweep)
load_system(mdl);
in = repmat(Simulink.SimulationInput(mdl), nC, 1);
for k = 1:nC
    in(k) = Simulink.SimulationInput(mdl);
    in(k) = in(k).setVariable("Kp_Speed", res.Kp(k));
    in(k) = in(k).setVariable("Ki_Speed", res.Ki(k));
end
out = sim(in, "UseFastRestart", "on", "ShowProgress", "off");

stepTimes = Ref_Period:Ref_Period:(Ref_StopTime - Ref_Period);   % the 150 rpm steps
for k = 1:nC
    m = staircaseMetrics(out(k), stepTimes, Ref_Step, Ref_Period, Ref_Start);
    res.OS_pct(k) = m.OS_max;  res.Ts_max(k) = m.Ts_max;  res.Ts_mean(k) = m.Ts_mean;
    res.Ess_rms(k) = m.Ess_rms; res.OS_first_pct(k) = m.OS_first;
    res.Pass(k) = m.OS_max < 10 && m.Ts_max < 0.5 && res.PM(k) >= 60 && res.GM_dB(k) >= 10 && m.OS_first < 10;
end
disp(res);

%% Selection
pass = find(res.Pass == 1);
if isempty(pass)
    error("design_SpeedPI:noCandidate", "No candidate meets the Gate 2 criteria.");
end
fast = pass(res.Ts_max(pass) < 0.3);
if ~isempty(fast), sel = fast(1); else, sel = pass(end); end
Kp_Speed = res.Kp(sel);  Ki_Speed = res.Ki(sel);  wc_sel = res.wc(sel);
fprintf("\nSelected wc = %g rad/s: Kp_Speed = %.5f, Ki_Speed = %.4f (PM %.1f deg, GM %.1f dB)\n", ...
    wc_sel, Kp_Speed, Ki_Speed, res.PM(sel), res.GM_dB(sel));
fprintf("Staircase: max OS %.1f %%, max settling %.3f s, mean settling %.3f s, steady-state rms error %.2f rpm, first-step OS %.1f %%\n", ...
    res.OS_pct(sel), res.Ts_max(sel), res.Ts_mean(sel), res.Ess_rms(sel), res.OS_first_pct(sel));

save(fullfile(impl, "SpeedPI_design.mat"), "res", "C", "sel", "Kp_Speed", "Ki_Speed", "wc_sel", "G", "H");

%% Plots for the selected controller
o  = out(sel);
r  = o.logsout.get("rpm_ref").Values;   y = o.logsout.get("rpm").Values;
yt = o.logsout.get("rpm_true").Values;  u = o.logsout.get("duty_cmd").Values;

fig1 = figure("Name", "M2 staircase", "Color", "w", "Position", [100 100 1000 700]);
tl1 = tiledlayout(fig1, 3, 1, "TileSpacing", "compact");
nexttile; plot(r.Time, r.Data, "k--"); hold on; plot(y.Time, y.Data, "LineWidth", 1.2); grid on;
ylabel("Speed [rpm]"); legend("rpm\_ref", "rpm (measured)", "Location", "best");
title(sprintf("M2 simulation – PI Kp = %.4f, Ki = %.3f (\\omega_c = %g rad/s)", Kp_Speed, Ki_Speed, wc_sel));
nexttile; plot(r.Time, r.Data - y.Data, "LineWidth", 1); grid on; ylabel("Error [rpm]"); ylim([-200 200]);
nexttile; plot(u.Time, u.Data, "LineWidth", 1.1); grid on; ylabel("Duty [%]"); xlabel("Time [s]");
exportgraphics(tl1, fullfile(supp, "M2_PISim_staircase.png"), "Resolution", 120);

fig2 = figure("Name", "M2 steps", "Color", "w", "Position", [100 100 1000 420]);
tl2 = tiledlayout(fig2, 1, 2, "TileSpacing", "compact");
for w = [10 20]   % one step down at +ve speed, one around zero crossing
    nexttile; idx = r.Time >= w - 0.2 & r.Time <= w + 1.0;
    plot(r.Time(idx), r.Data(idx), "k--"); hold on;
    plot(yt.Time(idx), yt.Data(idx), "LineWidth", 1.2); plot(y.Time(idx), y.Data(idx), "LineWidth", 1);
    grid on; xlabel("Time [s]"); ylabel("Speed [rpm]"); title(sprintf("Step at t = %g s", w));
    legend("ref", "rpm\_true", "rpm (measured)", "Location", "best");
end
exportgraphics(tl2, fullfile(supp, "M2_PISim_steps.png"), "Resolution", 120);

%% Local functions
function m = staircaseMetrics(out, stepTimes, stepSize, period, refStart)
%STAIRCASEMETRICS Overshoot / settling on each reference step (uses rpm_true).
r = out.logsout.get("rpm_ref").Values;  y = out.logsout.get("rpm_true").Values;
t = y.Time;  yv = squeeze(y.Data);  rv = squeeze(r.Data);
band = 0.05*stepSize;
OS = zeros(size(stepTimes));  Tset = OS;  ess = [];
for i = 1:numel(stepTimes)
    t0 = stepTimes(i);
    seg = t >= t0 & t < t0 + period;
    tr  = t(seg) - t0;  ys = yv(seg);  target = rv(find(seg, 1, "last"));
    dir = sign(target - rv(find(t < t0, 1, "last")));
    OS(i) = max(0, max(dir*(ys - target)))/stepSize*100;
    outside = find(abs(ys - target) > band, 1, "last");
    if isempty(outside), Tset(i) = 0; else, Tset(i) = tr(min(outside + 1, numel(tr))); end
    ess = [ess; ys(tr > period - 0.5) - target]; %#ok<AGROW>
end
% First (large, saturated) step from 0 to refStart
seg1 = t < period;
m.OS_first = max(0, max(yv(seg1) - refStart))/refStart*100;
m.OS_max = max(OS);  m.Ts_max = max(Tset);  m.Ts_mean = mean(Tset);  m.Ess_rms = rms(ess);
end
