import datetime
import plotly.graph_objects as go
import numpy as np

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
    
print(f"Time: {micros}")
print(f"Values: {values}")

fig = go.Figure()
fig.add_trace(go.Scatter(x = micros,y = values))
fig.show()