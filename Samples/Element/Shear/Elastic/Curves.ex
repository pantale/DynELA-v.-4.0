# Global parameters
Parameters, xrange=10, xname=$Horizontal\ displacement\ along\ x\ axis\ (mm)$, marksnumber=15, title=$One\ Element\ Elastic\ Shear\ Test$, crop=True

# TimeStep curve
timeStep, yname=$Timestep\ \Delta t$, legendlocate=topright, name=$DynELA\ \Delta t$, dt.plot, name=$Abaqus\ \Delta t$, Abaqus/Shear_timeStep.plot

# Comparison of von Mises stresses
vonMises, yname=$von\ Mises\ stress\ \overline{\sigma}$, legendposition=1.0:0.0, legendanchor=lower right, name=$DynELA\ \overline{\sigma}$, vonMises.plot, name=$Analytic\ \overline{\sigma}$, Analytic/Jaumann_mises.plot, name=$Abaqus\ \overline{\sigma}$, Abaqus/Shear_vonMises.plot

# Shear Stress XY comparison
stress_12, yname=$Shear\ stress\ \sigma_{12}$, legendlocate=bottomleft, name=$DynELA\ \sigma_{12}$, Stress.plot[0:4], name=$Analytic\ \sigma_{12}$, Analytic/Jaumann_s12.plot, name=$Abaqus\ \sigma_{12}$, Abaqus/Shear_StressXY.plot

# Normal Stress XX comparison
stress_11, yname=$Normal\ stress\ \sigma_{11}$, legendlocate=topleft, name=$DynELA\ \sigma_{11}$, Stress.plot[0:1], name=$Analytic\ \sigma_{11}$, Analytic/Jaumann_s11.plot, name=$Abaqus\ \sigma_{11}$, Abaqus/Shear_StressXX.plot

# Normal Stress YY comparison
stress_22, yname=$Normal\ stress\ \sigma_{22}$, legendlocate=bottomleft, name=$DynELA\ \sigma_{22}$, Stress.plot[0:2], name=$Analytic\ \sigma_{22}$, Analytic/Jaumann_s22.plot, name=$Abaqus\ \sigma_{22}$, Abaqus/Shear_StressYY.plot

# Normal Stress ZZ comparison
stress_33, yname=$Normal\ stress\ \sigma_{33}$, legendlocate=bottomleft, name=$DynELA\ \sigma_{33}$, Stress.plot[0:3], name=$Analytic\ \sigma_{33}$, Analytic/Jaumann_s33.plot, name=$Abaqus\ \sigma_{33}$, Abaqus/Shear_StressZZ.plot
