import control.matlab as ct
import numpy as np
import plotly.graph_objects as go

G = ct.tf([3497489.7],[1, 10934.02,115952.84])
time_model = np.linspace(0,7,701)
r = [3.1]*701
Y,time_model,x = ct.lsim(G,r,time_model)

fig = go.Figure()
fig.add_trace(go.Scatter(x = time_model, y = Y))
#fig.add_trace(go.Scatter(x = time_model, y = [83.78]*701))
fig.show()

