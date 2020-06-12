from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np

EPS = 0.001

def vec_repr(u):
    theta = np.arccos((u[0,0] + u[1,1]) / 2)
    n = (
        (u[0,1] + u[1, 0]) / (2 * 1j * np.sin(theta)),
        (u[0,1] - u[1, 0]) / (2 * np.sin(theta)),
        (u[1,1] - u[0, 0]) / (2 * 1j * np.sin(theta))
    )
    assert(theta.imag < EPS)
    assert(all(v.imag < EPS for v in n))

    print(u)
    print(tuple(v.real * theta.real for v in n))
    return theta.real, tuple(v.real for v in n)

hadamard = 1j / np.sqrt(2) * np.matrix([[1,  1],
                      [1, -1]])               
t_gate = np.matrix([[np.exp(-1j * np.pi / 8), 0],
                    [0,  np.exp(1j * np.pi / 8)]])
seed_gates = [hadamard, t_gate]
gates = [seed_gates]

for _ in range(0, 4):
    gates.append([g * s for g in gates[-1] for s in seed_gates])

pairs = (vec_repr(u) for u in gates[-1])
x, y, z = zip(*((theta * n[0], theta * n[1], theta * n[2]) for theta, n in pairs))

fig = plt.figure()
ax = plt.axes(projection='3d')

# pi sphere
phi = np.linspace(0, np.pi, 20)
theta = np.linspace(0, 2 * np.pi, 20)
phi, theta = np.meshgrid(phi, theta)
sx = np.pi * np.sin(phi) * np.cos(theta)
sy = np.pi * np.sin(phi) * np.sin(theta)
sz = np.pi * np.cos(phi)
ax.plot_surface(sx, sy, sz, color=(1, 1, 1, 0.1))


ax.scatter3D(x, y, z)

plt.show()
