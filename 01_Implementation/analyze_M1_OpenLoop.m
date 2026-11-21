function cal = analyze_M1_OpenLoop(runTag)
%ANALYZE_M1_OPENLOOP Evaluate the latest M1 Monitor & Tune run and compute calibration.
%   cal = analyze_M1_OpenLoop("run1") reads the latest Simulation Data
%   Inspector run of DCMotor_M1_OpenLoop, saves the signals to
%   01_Implementation/M1_OpenLoop_<runTag>_data.mat, saves a plot to
%   02_SupportingArtifacts/M1_OpenLoop_<runTag>.png and returns the
%   suggested calibration values for DCMotorParams.m:
%     Enc_Dir, Enc_Kcal  (assumption: 100 % PWM = Motor_NoLoadRPM = 2200 rpm,
%                         applied to the mean of the +100 % and -100 % speeds)
%     Cur_OffsetV, Cur_Dir
%
%   Profile (DCMotorParams OL_Time/OL_Duty): 0 % 0-2 s, +100 % 2-6 s,
%   0 % 6-9 s, -100 % 9-13 s, 0 % 13-18 s.

arguments
    runTag (1,1) string = "run1"
end

prj  = currentProject;
impl = fullfile(prj.RootFolder, "01_Implementation");
supp = fullfile(prj.RootFolder, "02_SupportingArtifacts");

% Parameter values that were active during the run
run(fullfile(impl, "DCMotorParams.m"));

%% Collect signals from the latest SDI run
sdiRun = Simulink.sdi.Run.getLatest;
names  = ["duty_cmd","rpm","rpm_raw","counts","current_A","sensor_V"];
data   = struct();
for n = names
    sig = getSignalsByName(sdiRun, n);
    if isempty(sig)
        error("analyze_M1_OpenLoop:missingSignal", "Signal '%s' not found in SDI run '%s'.", n, sdiRun.Name);
    end
    ts = sig(1).Values;
    data.(n) = timetable(seconds(ts.Time), double(ts.Data(:)), 'VariableNames', n);
end
tt = synchronize(data.duty_cmd, data.rpm, data.rpm_raw, data.counts, data.current_A, data.sensor_V, 'first', 'previous');
t  = seconds(tt.Time);

%% Steady-state windows (last 1.5 s of each segment)
wPos  = t >= 4.5  & t < 6.0;
wNeg  = t >= 11.5 & t < 13.0;
wZero = t >= 0.2  & t < 1.9;

rpmPos = mean(tt.rpm_raw(wPos));
rpmNeg = mean(tt.rpm_raw(wNeg));

% rpm_raw already contains Enc_Dir*Enc_Kcal -> correct relative to it
cal.Enc_Dir  = Enc_Dir * sign(rpmPos);
cal.Enc_Kcal = Enc_Kcal * Motor_NoLoadRPM / mean([abs(rpmPos) abs(rpmNeg)]);  % both directions
cal.rpmPos_meas   = rpmPos;
cal.rpmNeg_meas   = rpmNeg;
cal.asymmetry_pct = 100*(abs(rpmNeg) - abs(rpmPos))/abs(rpmPos);
cal.rpmPos_calibrated = abs(rpmPos)*cal.Enc_Kcal/Enc_Kcal;
cal.rpmNeg_calibrated = abs(rpmNeg)*cal.Enc_Kcal/Enc_Kcal;
cal.noise_raw_std  = std(tt.rpm_raw(wPos));
cal.noise_filt_std = std(tt.rpm(wPos));

% Encoder resolution check (counts per output rev from total travel)
cal.countsPerSec_pos = mean(diff(tt.counts(wPos)))/Ts;

% Current sensor
cal.Cur_OffsetV = mean(tt.sensor_V(wZero));
iPos = mean(tt.current_A(wPos));
iNeg = mean(tt.current_A(wNeg));
cal.Cur_Dir    = Cur_Dir * sign(iPos - iNeg + eps);
cal.iPos_A     = iPos;
cal.iNeg_A     = iNeg;
cal.iZero_noise_std = std(tt.current_A(wZero));

%% Save data and plot
save(fullfile(impl, "M1_OpenLoop_" + runTag + "_data.mat"), "tt", "cal");

fig = figure("Name", "M1 open loop " + runTag, "Color", "w", "Position", [100 100 900 750]);
tl = tiledlayout(fig, 3, 1, "TileSpacing", "compact");
nexttile; plot(t, tt.duty_cmd, "LineWidth", 1.2); grid on; ylabel("Duty [%]"); title("M1 open loop – " + runTag);
nexttile; plot(t, tt.rpm_raw, "Color", [0.7 0.7 0.7]); hold on; plot(t, tt.rpm, "LineWidth", 1.2);
yline([Motor_NoLoadRPM -Motor_NoLoadRPM], "--k"); grid on; ylabel("Speed [rpm]"); legend("rpm\_raw", "rpm (filtered)", "±2200", "Location", "best");
nexttile; plot(t, tt.current_A, "LineWidth", 1.2); grid on; ylabel("Current [A]"); xlabel("Time [s]");
exportgraphics(tl, fullfile(supp, "M1_OpenLoop_" + runTag + ".png"), "Resolution", 120);

%% Report
fprintf("\n=== M1 open-loop analysis (%s) ===\n", runTag);
fprintf("+100 %%: rpm_raw = %8.1f rpm | -100 %%: rpm_raw = %8.1f rpm | asymmetry = %+.1f %%\n", rpmPos, rpmNeg, cal.asymmetry_pct);
fprintf("Counts/s at +100 %% = %.0f  (expected %.0f at 2200 rpm)\n", cal.countsPerSec_pos, Motor_NoLoadRPM/60*Enc_CountsPerRev);
fprintf("Noise std (+100 %%): raw = %.1f rpm, filtered = %.1f rpm\n", cal.noise_raw_std, cal.noise_filt_std);
fprintf("Suggested: Enc_Dir = %+d, Enc_Kcal = %.4f  -> +100 %% = %.0f rpm, -100 %% = %.0f rpm\n", ...
    cal.Enc_Dir, cal.Enc_Kcal, cal.rpmPos_calibrated, cal.rpmNeg_calibrated);
fprintf("Current: offset = %.3f V, Cur_Dir = %+d, I(+100) = %.2f A, I(-100) = %.2f A, zero noise std = %.3f A\n", ...
    cal.Cur_OffsetV, cal.Cur_Dir, iPos, iNeg, cal.iZero_noise_std);
end
