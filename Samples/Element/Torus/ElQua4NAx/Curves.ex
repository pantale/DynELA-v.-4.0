# Global parameters
Parameters, xscale=1000, xname=$Horizontal\ displacement\ along\ x\ axis\ (mm)$, marksnumber=15, title=$One\ Element\ Torus\ Test$, crop=True

# Temperature curve
temperature, yname=$Temperature\ in\ ^{\circ}C$, legendlocate=bottomright, name=$DynELA\ T$, temperature.plot, name=$Abaqus\ T$, Abaqus/Torus_Temperature.plot

# Plastic strain curve
plasticStrain, yname=$Equivalent\ plastic\ strain\ \overline{\varepsilon}^{p}$, legendlocate=bottomright, name=$DynELA\ \overline{\varepsilon}^{p}$, plasticStrain.plot, name=$Abaqus\ \overline{\varepsilon}^{p}$, Abaqus/Torus_PlasticStrain.plot

# von Mises equivalent stress curve
vonMises, yname=$von\ Mises\ stress\ \overline{\sigma}$, legendlocate=bottomright, name=$DynELA\ \overline{\sigma}$, vonMises.plot, name=$Abaqus\ \overline{\sigma}$, Abaqus/Torus_vonMises.plot

# Density curve
density, yname=$Density$, legendlocate=bottomleft, name=$DynELA\ \rho$, density.plot, name=$Abaqus\ \rho$, Abaqus/Torus_density.plot

# TimeStep curve
timeStep, yname=$Time\ increment\ \Delta t$, legendlocate=bottomleft, name=$DynELA\ \Delta t$, dt.plot, name=$Abaqus\ \Delta t$, Abaqus/Torus_timeStep.plot

# Normal Stress XX curve
stress_11, yname=$Normal\ stress\ \sigma_{11}$, legendlocate=bottomright, name=$DynELA\ \sigma_{11}$, Stress.plot[0:1], name=$Abaqus\ \sigma_{11}$, Abaqus/Torus_StressXX.plot

# Normal Stress YY curve
stress_22, yname=$Normal\ stress\ \sigma_{22}$, legendlocate=bottomright, name=$DynELA\ \sigma_{22}$, Stress.plot[0:2], name=$Abaqus\ \sigma_{22}$, Abaqus/Torus_StressYY.plot

# Torus Stress ZZ curve
stress_33, yname=$Torus\ stress\ \sigma_{33}$, legendlocate=bottomright, name=$DynELA\ \sigma_{33}$, Stress.plot[0:3], name=$Abaqus\ \sigma_{33}$, Abaqus/Torus_StressZZ.plot

# Torus Stress XY curve
stress_12, yname=$Torus\ stress\ \sigma_{12}$, legendlocate=bottomright, name=$DynELA\ \sigma_{12}$, Stress.plot[0:4], name=$Abaqus\ \sigma_{12}$, Abaqus/Torus_StressXY.plot

# Torus Displacement X curve
dispX, yname=$Right\ edge\ displacement\ u_x$, legendlocate=bottomright, name=$DynELA\ u_x$, dispX.plot, name=$Abaqus\ u_x$, Abaqus/Torus_dispX.plot

# Plastic strains curves
plasticStrains, yname=$Equivalent\ plastic\ strain\ \overline{\varepsilon}^{p}$, legendlocate=bottomright, name=$DynELA\ \overline{\varepsilon}^{p}_1;\ \overline{\varepsilon}^{p}_4$, plasticStrains.plot[0:1], name=$DynELA\ \overline{\varepsilon}^{p}_2;\ \overline{\varepsilon}^{p}_3$, plasticStrains.plot[0:2], name=$Abaqus\ \overline{\varepsilon}^{p}_1;\ \overline{\varepsilon}^{p}_4$, Abaqus/Torus_PlasticStrains.plot[0:1], name=$Abaqus\ \overline{\varepsilon}^{p}_2;\ \overline{\varepsilon}^{p}_3$, Abaqus/Torus_PlasticStrains.plot[0:2]

