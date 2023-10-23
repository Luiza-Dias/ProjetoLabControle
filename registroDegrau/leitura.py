import datetime
import plotly.graph_objects as go
import numpy as np
import control.matlab as ct

file = open("degrau.txt")
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
    values.append((int(line[indexReal + 6 :])/60)*2*np.pi)
micros = list()
for t in timee:
    indexTime = t.find("time")
    micros.append(int(t[indexTime + 6 :])/1000000)

###Planta###
#G = ct.tf([3497489.7],[1, 10934.02,115952.84])
G = ct.tf([783718.87],[1, 10933.49,25982.76])
time_model = np.linspace(0,14,527)
r = [3.1]*527
Y,time_model,x = ct.lsim(G,r,time_model)
###Planta###

erro = list()
for i in range(len(Y)):
    erro.append(values[i] - Y[i])


referencia = [83.78]*527
referencia[0] = 0 
fig = go.Figure()
fig.add_trace(go.Scatter(x = micros,y = values, name = 'Leitura Encoder'))
fig.add_trace(go.Scatter(x = time_model, y = Y, name = 'Modelo Matematico'))
fig.add_trace(go.Scatter(x = time_model, y = referencia, name = 'Entrada em Degrau'))
fig.add_trace(go.Scatter(x = time_model,y = erro, name = "Erro"))
fig.update_layout(title='Resposta ao Degrau')
fig.show()