import datetime
import plotly.graph_objects as go
import numpy as np
import control.matlab as ct

file = open("senoide.txt")
vel = list()
timee = list()
while True:
    text = file.readline()
    if "Real" in text:
        vel.append(text)
    if "time" in text:
        timee.append(text)
    if not text:
        break

values = list()
for line in vel:
    #timee = datetime.time(int(line[0:2]), int(line[3:5]), )
    indexReal = line.find("Real")
    values.append(float(line[indexReal + 6 :]))
micros = list()
for t in timee:
    indexTime = t.find("time")
    micros.append(int(t[indexTime + 6 :])/1000000)
    
#print(f"Time: {micros}")
#(f"Values: {values}")

t2 = np.linspace(0,10,201)
sen = np.ones(201)
for i in range(len(t2)):
    sen[i] = np.sin(2*np.pi/120 * (i%120))*83

###Planta###
G = ct.tf([783718.87],[1, 10933.49,25982.76])
time_model = np.linspace(0,13,448)
r = np.ones(448)
for i in range(len(time_model)):
    r[i] = np.sin(2*np.pi/200 * (i%200))*3.1
Y,time_model,x = ct.lsim(G,r,time_model)
###Planta###

erro = list()
for i in range(448):
    erro.append(values[i] - Y[i])

fig = go.Figure()
#fig.add_trace(go.Scatter(x = t2,y = sen, name = 'Entrada Senoidal'))
fig.add_trace(go.Scatter(x = time_model, y = Y, name = 'Modelo Matematico'))
fig.add_trace(go.Scatter(x = micros,y = values, name = "Leitura Encoder"))
fig.add_trace(go.Scatter(x = time_model,y = erro, name = "Erro"))
fig.update_layout(title='Resposta a Entrada Senoidal')
fig.show()