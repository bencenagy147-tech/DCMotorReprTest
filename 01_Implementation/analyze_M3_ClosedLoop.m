function res = analyze_M3_ClosedLoop(runTag)
%ANALYZE_M3_CLOSEDLOOP Evaluate the latest M3 hardware run against Gate 3 and the M2 simulation.
%   res = analyze_M3_ClosedLoop("run1") reads the latest SDI run of
%   DCMotor_M3_ClosedLoopHW, computes per-step metrics on the measured speed,
%   simulates DCMotor_M2_PISim with the same gains for comparison, saves
%   01_Implementation/M3_<runTag>_data.mat and
%   02_SupportingArtifacts/M3_<runTag>.png, M3_<runTag>_steps.png.
%
%   Gate 3: |steady-state error| <= 30 rpm on every level, settling into the
%   +/-30 rpm gate tolerance < 1 s, smooth zero crossing, no sustained
%   oscillation. Settling into the stricter +/-5 % band (7.5 rpm) is reported
%   too; on hardware it is sensitive to single 8-13 rpm speed spikes.

arguments
    runTag (1,1) string = "run1"
end

prj  = currentProject;
impl = fullfile(prj.RootFolder, "01_Implementation");
supp = fullfile(prj.RootFolder, "02_SupportingArtifacts");
run(fullfile(impl, "DCMotorParams.m"));

%% Hardware data from SDI (latest run of the hardware model, not the comparison sim)
runIDs = Simulink.sdi.getAllRunIDs;
sdiRun = [];
for k = numel(runIDs):-1:1
    candidate = Simulink.sdi.getRun(runIDs(k));
    if strcmp(candidate.Model, "DCMotor_M3_ClosedLoopHW"), sdiRun = candidate; break; end
end
if isempty(sdiRun)
    error("analyze_M3_ClosedLoop:noRun", "No SDI run of DCMotor_M3_ClosedLoopHW found.");
end
names  = ["rpm_ref","rpm","rpm_raw","duty_cmd","current_A"];
parts  = cell(1, numel(names));
for i = 1:numel(names)
    sig = getSignalsByName(sdiRun, names(i));
    if isempty(sig)
        error("analyze_M3_ClosedLoop:missingSignal", "Signal '%s' not found in SDI run '%s'.", names(i), sdiRun.Name);
    end
    ts = sig(1).Values;
    parts{i} = timetable(seconds(ts.Time), double(ts.Data(:)), 'VariableNames', names(i));
end
tt = synchronize(parts{:}, 'first', 'previous');
t  = seconds(tt.Time);

%% Simulation with the same gains (M2 model)
in  = Simulink.SimulationInput("DCMotor_M2_PISim");
in  = in.setVariable("Kp_Speed", Kp_Speed);
in  = in.setVariable("Ki_Speed", Ki_Speed);
out = sim(in);
ySim = out.logsout.get("rpm").Values;

%% Per-step metrics (measured rpm)
stepTimes = 0:Ref_Period:(Ref_StopTime - Ref_Period);   % 21 levels incl. the start
band = 0.05*Ref_Step;
nL = numel(stepTimes);
band30 = 30;
lvl = table('Size', [nL 8], 'VariableTypes', repmat("double", 1, 8), ...
    'VariableNames', ["t0","ref","ess_mean","ess_rms","overshoot_rpm","settle_s","settle30_s","duty_mean"]);
for i = 1:nL
    t0  = stepTimes(i);
    seg = t >= t0 & t < t0 + Ref_Period;
    tr  = t(seg) - t0;  y = tt.rpm(seg);  r = tt.rpm_ref(seg);  target = r(end);
    if i == 1, prev = 0; else, prev = tt.rpm_ref(find(t < t0, 1, "last")); end
    dir = sign(target - prev);
    ssw = tr >= Ref_Period - 0.5;
    lvl.t0(i) = t0;  lvl.ref(i) = target;
    lvl.ess_mean(i) = mean(y(ssw) - target);
    lvl.ess_rms(i)  = rms(y(ssw) - target);
    lvl.overshoot_rpm(i) = max(0, max(dir*(y - target)));
    outside = find(abs(y - target) > band, 1, "last");
    if isempty(outside), lvl.settle_s(i) = 0; else, lvl.settle_s(i) = tr(min(outside + 1, numel(tr))); end
    outside = find(abs(y - target) > band30, 1, "last");
    if isempty(outside), lvl.settle30_s(i) = 0; else, lvl.settle30_s(i) = tr(min(outside + 1, numel(tr))); end
    lvl.duty_mean(i) = mean(tt.duty_cmd(seg & t >= t0 + Ref_Period - 0.5));
end
stepsOnly = lvl(2:end, :);   % 150 rpm steps (exclude the 0 -> 1500 start)

res.levels         = lvl;
res.ess_max_abs    = max(abs(lvl.ess_mean));
res.ess_rms_max    = max(lvl.ess_rms);
res.settle_max     = max(stepsOnly.settle_s);      % +/-7.5 rpm band
res.settle_mean    = mean(stepsOnly.settle_s);
res.settle30_max   = max(stepsOnly.settle30_s);    % +/-30 rpm gate band
res.settle30_mean  = mean(stepsOnly.settle30_s);
res.os_max_pct     = max(stepsOnly.overshoot_rpm)/Ref_Step*100;
res.os_first_pct   = lvl.overshoot_rpm(1)/Ref_Start*100;
res.duty_peak      = max(abs(tt.duty_cmd));
res.rms_track      = rms(tt.rpm(t < Ref_StopTime) - tt.rpm_ref(t < Ref_StopTime));
yS = interp1(ySim.Time, squeeze(ySim.Data), t(t < Ref_StopTime), "previous", "extrap");
res.rms_hw_vs_sim  = rms(tt.rpm(t < Ref_StopTime) - yS);
res.gate.ess       = res.ess_max_abs <= 30;
res.gate.settle    = res.settle30_max < 1;
res.gate.zero      = abs(lvl.ess_mean(lvl.ref == 0)) <= 30 && lvl.ess_rms(lvl.ref == 0) <= 30;
res.gate.pass      = res.gate.ess && res.gate.settle && res.gate.zero;
res.Kp_Speed = Kp_Speed;  res.Ki_Speed = Ki_Speed;

save(fullfile(impl, "M3_" + runTag + "_data.mat"), "tt", "res");

%% Plots
fig = figure("Name", "M3 " + runTag, "Color", "w", "Position", [100 100 1000 820]);
tl = tiledlayout(fig, 4, 1, "TileSpacing", "compact");
nexttile; plot(t, tt.rpm_ref, "k--"); hold on; plot(ySim.Time, squeeze(ySim.Data), "Color", [0.6 0.6 0.6]);
plot(t, tt.rpm, "LineWidth", 1.2); grid on; ylabel("Speed [rpm]");
legend("rpm\_ref", "M2 simulation", "hardware rpm", "Location", "best");
title(sprintf("M3 closed loop – %s (Kp = %.4f, Ki = %.3f)", runTag, Kp_Speed, Ki_Speed));
nexttile; plot(t, tt.rpm_ref - tt.rpm, "LineWidth", 1); grid on; ylabel("Error [rpm]"); ylim([-200 200]);
nexttile; plot(t, tt.duty_cmd, "LineWidth", 1.1); grid on; ylabel("Duty [%]");
nexttile; plot(t, tt.current_A, "LineWidth", 1); grid on; ylabel("Current [A]"); xlabel("Time [s]");
exportgraphics(tl, fullfile(supp, "M3_" + runTag + ".png"), "Resolution", 120);

fig2 = figure("Name", "M3 steps " + runTag, "Color", "w", "Position", [100 100 1100 400]);
tl2 = tiledlayout(fig2, 1, 3, "TileSpacing", "compact");
for w = [10 20 22]
    nexttile; idx = t >= w - 0.2 & t <= w + 1.2; idxS = ySim.Time >= w - 0.2 & ySim.Time <= w + 1.2;
    plot(t(idx), tt.rpm_ref(idx), "k--"); hold on;
    plot(ySim.Time(idxS), squeeze(ySim.Data(idxS)), "Color", [0.6 0.6 0.6]);
    plot(t(idx), tt.rpm(idx), "LineWidth", 1.2); grid on; xlabel("Time [s]"); ylabel("Speed [rpm]");
    title(sprintf("Step at t = %g s", w)); legend("ref", "sim", "hardware", "Location", "best");
end
exportgraphics(tl2, fullfile(supp, "M3_" + runTag + "_steps.png"), "Resolution", 120);

%% Report
fprintf("\n=== M3 closed-loop analysis (%s) ===\n", runTag);
disp(lvl);
fprintf("Max |steady-state error| = %.1f rpm, max level rms = %.1f rpm\n", res.ess_max_abs, res.ess_rms_max);
fprintf("150 rpm steps: settling +/-30 rpm max %.2f s (mean %.2f s) | +/-7.5 rpm max %.2f s (mean %.2f s)\n", ...
    res.settle30_max, res.settle30_mean, res.settle_max, res.settle_mean);
fprintf("Max overshoot %.1f %% of step; first step overshoot %.1f %%\n", res.os_max_pct, res.os_first_pct);
fprintf("Peak duty %.1f %%, overall tracking rms %.1f rpm, hardware vs simulation rms %.1f rpm\n", ...
    res.duty_peak, res.rms_track, res.rms_hw_vs_sim);
fprintf("Gate 3: ess %d | settling %d | zero crossing %d  ->  PASS = %d\n", ...
    res.gate.ess, res.gate.settle, res.gate.zero, res.gate.pass);
end
