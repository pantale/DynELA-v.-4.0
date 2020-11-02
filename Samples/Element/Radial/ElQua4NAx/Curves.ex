# Global parameters
Parameters, xscale=1000, xname=$Horizontal\ displacement\ along\ x\ axis\ (mm)$, marksnumber=15, title=$One\ Element\ Radial\ Test$, crop=True

# Temperature curve
temperature, yname=$Temperature\ in\ ^{\circ}C$, legendlocate=bottomright, name=$DynELA\ T$, temperature.plot, name=$Abaqus\ T$, Abaqus/Radial_Temperature.plot

# Plastic strain curve
plasticStrain, yname=$Equivalent\ plastic\ strain\ \overline{\varepsilon}^{p}$, legendlocate=bottomright, name=$DynELA\ \overline{\varepsilon}^{p}$, plasticStrain.plot, name=$Abaqus\ \overline{\varepsilon}^{p}$, Abaqus/Radial_PlasticStrain.plot

# von Mises equivalent stress curve
vonMises, yname=$von\ Mises\ stress\ \overline{\sigma}$, legendlocate=bottomright, name=$DynELA\ \overline{\sigma}$, vonMises.plot, name=$Abaqus\ \overline{\sigma}$, Abaqus/Radial_vonMises.plot

# Density curve
density, yname=$Density$, legendlocate=bottomleft, name=$DynELA\ \rho$, density.plot, name=$Abaqus\ \rho$, Abaqus/Radial_density.plot

# TimeStep curve
timeStep, yname=$Time\ increment\ \Delta t$, legendlocate=bottomleft, name=$DynELA\ \Delta t$, dt.plot, name=$Abaqus\ \Delta t$, Abaqus/Radial_timeStep.plot

# Normal Stress XX curve
stress_11, yname=$Normal\ stress\ \sigma_{11}$, legendlocate=bottomright, name=$DynELA\ \sigma_{11}$, Stress.plot[0:1], name=$Abaqus\ \sigma_{11}$, Abaqus/Radial_StressXX.plot

# Normal Stress YY curve
stress_22, yname=$Normal\ stress\ \sigma_{22}$, legendlocate=bottomright, name=$DynELA\ \sigma_{22}$, Stress.plot[0:2], name=$Abaqus\ \sigma_{22}$, Abaqus/Radial_StressYY.plot

# Radial Stress ZZ curve
stress_33, yname=$Radial\ stress\ \sigma_{33}$, legendlocate=bottomright, name=$DynELA\ \sigma_{33}$, Stress.plot[0:3], name=$Abaqus\ \sigma_{33}$, Abaqus/Radial_StressZZ.plot

# Radial Stress XY curve
stress_12, yname=$Radial\ stress\ \sigma_{12}$, legendlocate=bottomright, name=$DynELA\ \sigma_{12}$, Stress.plot[0:4], name=$Abaqus\ \sigma_{12}$, Abaqus/Radial_StressXY.plot

