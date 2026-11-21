%VERIFY_FRICTIONFF Simulate the staircase with/without friction model and feedforward.
%   Cases (DCMotor_M2_PISim, SimulationInput overrides):
%     A: no friction, no FF   (original M2 design)
%     B: friction,    no FF   (should reproduce the M3 run1 breakaway overshoot)
%     C: friction,    FF      (proposed fix)
%     D: no friction, FF
%   Metrics on rpm_true: max overshoot on 150 rpm steps, overshoot of the
%   0 -> -150 rpm breakaway step (t = 22 s), max settling into +/-30 rpm.
%   Saves 02_SupportingArtifacts/M3_FrictionFF_sim.png and
%   01_Implementation/FrictionFF_sim.mat.

prj  = currentProject;
impl = fullfile(prj.RootFolder, "01_Implementation");
supp = fullfile(prj.RootFolder, "02_SupportingArtifacts");
mdl  = "DCMotor_M2_PISim";
DCMotorParams;

cases = ["A: no friction, PI only", "B: friction, PI only", "C: friction, PI + FF", "D: no friction, PI + FF"];
fric  = [0 1 1 0];
ffen  = [0 0 1 1];
in = repmat(Simulink.SimulationInput(mdl), numel(cases), 1);
for k = 1:numel(cases)
    in(k) = Simulink.SimulationInput(mdl);
    in(k) = in(k).setVariable("Sim_FrictionEnable", fric(k));
    in(k) = in(k).setVariable("FF_Enable", ffen(k));
end
out = sim(in, "ShowProgress", "off");

stepTimes = Ref_Period:Ref_Period:(Ref_StopTime - Ref_Period);
M = table('Size', [numel(cases) 5], 'VariableTypes', ["string","double","double","double","double"], ...
    'VariableNames', ["case","OS_max_rpm","OS_breakaway_rpm","settle30_max_s","ess_max_rpm"]);
for k = 1:numel(cases)
    y = out(k).logsout.get("rpm_true").Values;  r = out(k).logsout.get("rpm_ref").Values;
    t = y.Time;  yv = squeeze(y.Data);  rv = squeeze(r.Data);
    os = zeros(size(stepTimes));  ts30 = os;  ess = os;
    for i = 1:numel(stepTimes)
        t0 = stepTimes(i);  seg = t >= t0 & t < t0 + Ref_Period;  tr = t(seg) - t0;
        target = rv(find(seg, 1, "last"));  prev = rv(find(t < t0, 1, "last"));
        e = yv(seg) - target;
        os(i) = max(0, max(sign(target - prev)*e));
        o = find(abs(e) > 30, 1, "last");  if isempty(o), ts30(i) = 0; else, ts30(i) = tr(min(o + 1, numel(tr))); end
        ess(i) = abs(mean(e(tr > Ref_Period - 0.5)));
    end
    M.case(k) = cases(k);  M.OS_max_rpm(k) = max(os);  M.OS_breakaway_rpm(k) = os(stepTimes == 22);
    M.settle30_max_s(k) = max(ts30);  M.ess_max_rpm(k) = max(ess);
end
disp(M);
save(fullfile(impl, "FrictionFF_sim.mat"), "M");

fig = figure("Name", "Friction FF sim", "Color", "w", "Position", [100 100 1000 420]);
tl = tiledlayout(fig, 1, 2, "TileSpacing", "compact");
for w = [20 22]
    nexttile; hold on; grid on;
    r = out(1).logsout.get("rpm_ref").Values;  idx = r.Time >= w - 0.2 & r.Time <= w + 1.2;
    plot(r.Time(idx), squeeze(r.Data(idx)), "k--");
    for k = 1:3
        y = out(k).logsout.get("rpm_true").Values;  plot(y.Time(idx), squeeze(y.Data(idx)), "LineWidth", 1.2);
    end
    xlabel("Time [s]"); ylabel("Speed [rpm]"); title(sprintf("Step at t = %g s", w));
    legend(["ref", cases(1:3)], "Location", "best");
end
exportgraphics(tl, fullfile(supp, "M3_FrictionFF_sim.png"), "Resolution", 120);
