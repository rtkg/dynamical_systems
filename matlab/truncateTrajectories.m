function trajectories=truncateTrajectories(trajectories,v_thresh,alpha_T,plot_flag)

N=size(trajectories.joints,2);

id_onset=Inf;
id_end=0;

for i=1:N   
      
    %do nothing if the velocity is zero over the whole duration or the
    %joint is set to be ignored
    if((max(abs(trajectories.joints(i).yd)) < 1e-2) )
        continue;
    end

    
    id=find(abs(trajectories.joints(i).yd) > max(abs(trajectories.joints(i).yd))*v_thresh/100);
    T=trajectories.t(id(end))-trajectories.t(id(1));
    id_s=find(trajectories.t >= trajectories.t(id(1))-T/2*(alpha_T-1));
    id_e=find(trajectories.t >= trajectories.t(id(end))+T/2*(alpha_T-1));

     if isempty(id_e)
         id_e=length(trajectories.t);
     end
    
     trajectories.joints(i).t_s=trajectories.t(id_s(1));
     trajectories.joints(i).t_e=trajectories.t(id_e(1));
     
    if(id_s(1) < id_onset)
        id_onset=id_s(1);
        limit_joints(1)=trajectories.joints(i).name;
    end
    if(id_e(1) > id_end)
        id_end=id_e(1);
        limit_joints(2)=trajectories.joints(i).name;
    end
end

disp('Limiting onset joint: '); disp(limit_joints(1));
disp('Limiting end joint: '); disp(limit_joints(2));


id_input_onset=find(trajectories.t_input >= trajectories.t(id_onset)); id_input_onset=id_input_onset(1);
id_input_end=find(trajectories.t_input >= trajectories.t(id_end)); id_input_end=id_input_end(1);

t_cutoff=trajectories.t(id_onset)-trajectories.t(1);
trajectories.t=trajectories.t(id_onset:id_end); trajectories.t=trajectories.t-trajectories.t(1);
trajectories.t_input=trajectories.t_input(id_input_onset:id_input_end); trajectories.t_input=trajectories.t_input-trajectories.t_input(1);
for i=1:N
    trajectories.joints(i).y=trajectories.joints(i).y(id_onset:id_end);
    trajectories.joints(i).yd=trajectories.joints(i).yd(id_onset:id_end);
    trajectories.joints(i).ydd=trajectories.joints(i).ydd(id_onset:id_end);
    trajectories.joints(i).y_input=trajectories.joints(i).y_input(id_input_onset:id_input_end);
    
    if ~isempty(trajectories.joints(i).t_s)
        trajectories.joints(i).t_s=trajectories.joints(i).t_s-t_cutoff;
    else
        trajectories.joints(i).t_s=trajectories.t(1);
    end
    
     if ~isempty(trajectories.joints(i).t_e)
        trajectories.joints(i).t_e=trajectories.joints(i).t_e-t_cutoff;
    else
        trajectories.joints(i).t_e=trajectories.t(end);
     end
    
end

if plot_flag
    for i=1:N
        id_t_s=find(trajectories.t >= trajectories.joints(i).t_s); id_t_s=id_t_s(1);
        id_t_e=find(trajectories.t >= trajectories.joints(i).t_e); id_t_e=id_t_e(1);
        subplot(3,1,1);
        plot(trajectories.t,trajectories.joints(i).y,'b');
        grid on; hold on;
        plot(trajectories.t_input,trajectories.joints(i).y_input,'r');
        plot([trajectories.t(id_t_s) trajectories.t(id_t_e)], [trajectories.joints(i).y(id_t_s) trajectories.joints(i).y(id_t_e)],'ko','MarkerFaceColor','r');
        title(strcat(trajectories.joints(i).name,' position'));
        legend('smoothened data','input data','onset/end time','Location','SouthEast');
        subplot(3,1,2);
        plot(trajectories.t,trajectories.joints(i).yd,'m');
        grid on; hold on;
        plot([trajectories.t(id_t_s) trajectories.t(id_t_e)], [trajectories.joints(i).yd(id_t_s) trajectories.joints(i).yd(id_t_e)],'ko','MarkerFaceColor','r');
        title(strcat(trajectories.joints(i).name,' velocity'));
        subplot(3,1,3);
        plot(trajectories.t,trajectories.joints(i).ydd,'g');
        grid on; hold on;
        plot([trajectories.t(id_t_s) trajectories.t(id_t_e)], [trajectories.joints(i).ydd(id_t_s) trajectories.joints(i).ydd(id_t_e)],'ko','MarkerFaceColor','r');
        title(strcat(trajectories.joints(i).name,' acceleration'));
        keyboard;
        close all;
    end
end






