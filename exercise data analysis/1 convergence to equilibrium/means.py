import matplotlib.pyplot as plt
import pandas as pd
import numpy as np



df = pd.read_csv('Exercise_1_diff_temp.csv')

df["Mag_mean"] = 0.0
df["Mag_max"] = 0.0
df["Mag_min"] = 0.0
df["E_mean"] = 0.0
df["E_max"] = 0.0
df["E_min"] = 0.0

for index, row in df.iterrows():

    
    Mag_m = 0.0
    Mag_max = -1.0
    Mag_min = 1.0
    E_m = 0.0
    E_max = 0.0
    E_min = 0.0
    i=0.0
    a=0.0
    while i < len(df):
        if ((abs(df.Beta[i]) == abs(row['Beta'])) and (abs(df.Sweep[i]) == abs(row['Sweep']))):
            
            if (df.Mag[i] > Mag_max):
                Mag_max = df.Mag[i]
            
            if (df.Mag[i] < Mag_min):
                Mag_min = df.Mag[i]
            
            if (df.E_N[i] > E_max):
                E_max = df.Mag[i]
                
            if (df.E_N[i] > E_min):
                E_min = df.E_N[i]
            
            
            Mag_m = Mag_m + df.Mag[i]
            E_m = E_m + df.E_N[i]
            a = a+1
            i = i+1
        else:
            i = i+1

    df.at[index, 'Mag_mean'] = Mag_m/a
    df.at[index, 'Mag_max'] = Mag_max
    df.at[index, 'Mag_min'] = Mag_min
    
    df.at[index, 'E_mean'] = E_m/a
    df.at[index, 'E_max'] = E_max
    df.at[index, 'E_min'] = E_min
    
    
    #print (row['B_mean'])
    
print(df)
df.to_csv("Exercise_1_diff_temp_mean.csv", index=True)