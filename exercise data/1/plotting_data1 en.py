import matplotlib.pyplot as plt
import pandas as pd
import numpy as np


df = pd.read_csv('data_1.csv')

sweep = df["Sweep"]
e_25 = df["0.25e"]
e_35 = df["0.35e"]
e_4 = df["0.4e"]
e_45 = df["0.45e"]
e_5= df["0.5e"]
e_6= df["0.6e"]
e_7= df["0.7e"]

#B = df["B"]
#B_error = df["B_error"]

#r=np.linspace(0,0.007,1000)
#m_=1737454.063
#cplus_=280.736708
#map_fit = (m_*r)+cplus_

#plt.errorbar(B,T,xerr = B_error,fmt ='ks',capsize=4, elinewidth=1.5,markersize=6,label='Points')#,label='1$^{st}$ Cluster',markersize=5)       
plt.plot(sweep,e_25/2,'k',markersize=7,label='β = 0.25')
plt.plot(sweep,e_35/2,'g',markersize=7,label='β = 0.35')
plt.plot(sweep,e_4/2,'r',markersize=7,label='β = 0.4')
plt.plot(sweep,e_5/2,'b',markersize=7,label='β = 0.5')
plt.plot(sweep,e_6/2,'m',markersize=7,label='β = 0.6')
#plt.plot(x,z0,'r',markersize=7,label='y=0')
#plt.plot(r,map_fit,'r--',label='Trendline',linewidth=2.5)  

plt.show
plt.xlabel('Iterations',fontsize=10)          #labels x axis as 'r (μm)'
plt.ylabel('⟨E/JN⟩',fontsize=10)    #labels y axis as 'Intensity (Wm^-2)'
plt.legend(loc='right') 
plt.xlim(-1,500)                 #x axis is from -25 to 25 μm
#plt.ylim(-4.05,0.1) 

plt.tick_params(bottom=True, top=True, left=True, right=True)
plt.tick_params(axis="x", direction="in", length=5, width=1)
plt.tick_params(axis="y", direction="in", length=5, width=1)

plt.xticks([100,200,300,400,500])#,['7.5', '10','12.5', '15', '17.5'])
plt.yticks([-2,-1.5,-1,-0.5])#,['1.1','1.15', '1.2','1.25','1.3'])# '1.4'])
#plt.set_xticklabels(['7.5', '10','12.5', '15', '17.5'])