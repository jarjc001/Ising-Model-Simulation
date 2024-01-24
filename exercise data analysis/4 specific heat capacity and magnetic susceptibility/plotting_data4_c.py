import matplotlib.pyplot as plt
import pandas as pd
import numpy as np


df = pd.read_csv('Exercise_4_100steps.csv')

T_Tc = df["T_Tc"]
rel_c = df["relative_c"]
mf_t_tc = df["mf_T/T_C"]
mf_rel_c = df["mf_c"]


#B = df["B"]
#B_error = df["B_error"]

#r=np.linspace(0,0.007,1000)
#m_=1737454.063
#cplus_=280.736708
#map_fit = (m_*r)+cplus_

#plt.errorbar(Temp,E_N_mean,yerr = mag_sd,fmt ='ks',capsize=4, elinewidth=1.5,markersize=6,label='Points')#,label='1$^{st}$ Cluster',markersize=5)       

plt.plot(T_Tc,rel_c,'k.',markersize=7,label='Ising',linewidth=2.5)
plt.plot(mf_t_tc,mf_rel_c,'r',markersize=7,label='Mean-Field',linewidth=2.5)


plt.show
plt.xlabel('T/T$_c$',fontsize=12)           #labels x axis as 'r (μm)'
plt.legend()
plt.ylabel('c/c$_{max}$',fontsize=12)    #labels y axis as 'Intensity (Wm^-2)'
#plt.legend(loc='upper right') 
#plt.xlim(1,4)                   #x axis is from -25 to 25 μm
#plt.ylim(-2.05,-0.5) 

#plt.axline((2.27, 0.811618), (2.27, 0.411642),color='k',linestyle="--",linewidth=2) 

plt.tick_params(bottom=True, top=True, left=True, right=True)
plt.tick_params(axis="x", direction="in", length=5, width=1)
plt.tick_params(axis="y", direction="in", length=5, width=1)

#plt.xticks([1.5,2,2.27,2.5,3,3.5,4],['1.5', '2', 'T$_c$', '2.5', '3', '3.5', '4'])
#plt.yticks([0.6,0.8,1,1.2,1.4,1.6,1.8,2])#,['1.1','1.15', '1.2','1.25','1.3'])# '1.4'])
#plt.set_xticklabels(['7.5', '10','12.5', '15', '17.5'])