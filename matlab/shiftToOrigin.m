function trajectories=shiftToOrigin(trajectories)

for i=1:numel(trajectories.joints)
   trajectories.joints(i).y=trajectories.joints(i).y-trajectories.joints(i).y(end);
end



