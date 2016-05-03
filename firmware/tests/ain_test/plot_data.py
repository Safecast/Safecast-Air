from __future__ import print_function
import sys
import scipy
import matplotlib.pyplot as plt

datafile = sys.argv[1]

data = scipy.loadtxt(datafile)

vscp = data[:,0]
vain = data[:,1]

fit = scipy.polyfit(vscp, vain, 1)
vscp_fit = scipy.linspace(vscp.min(), vscp.max(),500)
vain_fit = scipy.polyval(fit,vscp_fit)

slope = fit[0]
offset = fit[1]

plt.plot(vscp_fit, vain_fit,'r')
plt.plot(vscp, vain,'o')
plt.grid('on')
plt.xlabel('Scope Voltage (V)')
plt.ylabel('Wrk 1 (V)')
plt.text(0.2,4.5,'slope={0:1.3f}'.format(slope))
plt.text(0.2,4.0,'slope={0:1.3f}'.format(offset))
plt.show()
