# Global parameters
Parameters, xscale=1000, xname=$Horizontal\ displacement\ along\ x\ axis\ (mm)$, marksnumber=15, title=$One\ Element\ Shear\ Test$, crop=True

# Temperature curve
temperature, yname=$Temperature\ in\ ^{\circ}C$, legendlocate=bottomright, name=$DynELA\ T$, temperature.plot, name=$Abaqus\ T$, Abaqus/Shear_Temperature.plot

# Plastic strain curve
plasticStrain, yname=$Equivalent\ plastic\ strain\ \overline{\varepsilon}^{p}$, legendlocate=bottomright, name=$DynELA\ \overline{\varepsilon}^{p}$, plasticStrain.plot, name=$Abaqus\ \overline{\varepsilon}^{p}$, Abaqus/Shear_PlasticStrain.plot

# von Mises equivalent stress curve
vonMises, yname=$von\ Mises\ stress\ \overline{\sigma}$, legendlocate=topright, name=$DynELA\ \overline{\sigma}$, vonMises.plot, name=$Abaqus\ \overline{\sigma}$, Abaqus/Shear_vonMises.plot

# Density curve
density, yname=$Density$, legendlocate=bottomright, name=$DynELA\ \rho$, density.plot, name=$Abaqus\ \rho$, Abaqus/Shear_density.plot

# TimeStep curve
timeStep, yname=$Time\ increment\ \Delta t$, legendlocate=topright, name=$DynELA\ \Delta t$, dt.plot, name=$Abaqus\ \Delta t$, Abaqus/Shear_timeStep.plot

# Normal Stress XX curve
stress_11, yname=$Normal\ stress\ \sigma_{11}$, legendlocate=bottomleft, name=$DynELA\ \sigma_{11}$, Stress.plot[0:1], name=$Abaqus\ \sigma_{11}$, Abaqus/Shear_StressXX.plot

# Normal Stress YY curve
stress_22, yname=$Normal\ stress\ \sigma_{22}$, legendlocate=bottomleft, name=$DynELA\ \sigma_{22}$, Stress.plot[0:2], name=$Abaqus\ \sigma_{22}$, Abaqus/Shear_StressYY.plot

# Shear Stress ZZ curve
stress_33, yname=$Shear\ stress\ \sigma_{33}$, legendlocate=bottomleft, name=$DynELA\ \sigma_{33}$, Stress.plot[0:3], name=$Abaqus\ \sigma_{33}$, Abaqus/Shear_StressZZ.plot

# Shear Stress XY curve
stress_12, yname=$Shear\ stress\ \sigma_{12}$, legendlocate=topright, name=$DynELA\ \sigma_{12}$, Stress.plot[0:4], name=$Abaqus\ \sigma_{12}$, Abaqus/Shear_StressXY.plot

