import matplotlib.pyplot as plt
import pandas as pd
import numpy as np


df = pd.read_csv('data_1.csv')

sweep = df["Sweep"]
m_25 = df["0.25m"]
m_35 = df["0.35m"]
m_4 = df["0.4m"]
m_45 = df["0.45m"]
m_5= df["0.5m"]
m_6= df["0.6m"]


#B = df["B"]
#B_error = df["B_error"]

#r=np.linspace(0,0.007,1000)
#m_=1737454.063
#cplus_=280.736708
#map_fit = (m_*r)+cplus_

#plt.errorbar(B,T,xerr = B_error,fmt ='ks',capsize=4, elinewidth=1.5,markersize=6,label='Points')#,label='1$^{st}$ Cluster',markersize=5)       
plt.plot(sweep,m_25,'k',markersize=7,label='β = 0.25')
plt.plot(sweep,m_35,'g',markersize=7,label='β = 0.35')
plt.plot(sweep,m_4,'r',markersize=7,label='β = 0.4')
plt.plot(sweep,m_5,'b',markersize=7,label='β = 0.5')
plt.plot(sweep,m_6,'m',markersize=7,label='β = 0.6')
#plt.plot(x,z0,'r',markersize=7,label='y=0')
#plt.plot(r,map_fit,'r--',label='Trendline',linewidth=2.5)  

plt.show
plt.xlabel('Iterations',fontsize=10)          #labels x axis as 'r (μm)'
plt.ylabel('⟨M⟩',fontsize=10)    #labels y axis as 'Intensity (Wm^-2)'
plt.legend() 
plt.xlim(-1,500)                 #x axis is from -25 to 25 μm
plt.ylim(-1.01,0.1) 

plt.tick_params(bottom=True, top=True, left=True, right=True)
plt.tick_params(axis="x", direction="in", length=5, width=1)
plt.tick_params(axis="y", direction="in", length=5, width=1)

#plt.axline((0, 0), (500, 0),color='k') 

plt.xticks([100,200,300,400,500])
plt.yticks([-1,-0.8,-0.6,-0.4,-0.2,0])#,['1.1','1.15', '1.2','1.25','1.3'])# '1.4'])
#plt.set_xticklabels(['7.5', '10','12.5', '15', '17.5'])