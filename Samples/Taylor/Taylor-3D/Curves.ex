# Global parameters
Parameters, xname=$Time\ (s)$, marksnumber=15, title=$Taylor\ 3D\ Benchmark\ Test$, crop=True

# Temperature curve
temperature, yname=$Temperature\ (^{\circ}C)$, legendlocate=bottomright, name=$DynELA\ T$, temperature.plot, name=$Abaqus\ T$, Abaqus/Taylor_Temperature.plot

# Plastic strain curve
plasticStrain, yname=$Equivalent\ plastic\ strain\ \overline{\varepsilon}^{p}$, legendlocate=bottomright, name=$DynELA\ \overline{\varepsilon}^{p}$, plasticStrain.plot, name=$Abaqus\ \overline{\varepsilon}^{p}$, Abaqus/Taylor_PlasticStrain.plot

# von Mises equivalent stress curve
vonMises, yname=$von\ Mises\ stress\ \overline{\sigma}\ (MPa)$, legendlocate=topright, name=$DynELA\ \overline{\sigma}$, vonMises.plot, name=$Abaqus\ \overline{\sigma}$, Abaqus/Taylor_vonMises.plot

# TimeStep curve
timeStep, yname=$Time\ increment\ \Delta t\ (s)$, legendlocate=topright, name=$DynELA\ \Delta t$, dt.plot, name=$Abaqus\ \Delta t$, Abaqus/Taylor_timeStep.plot

# Kinetic energy curve
kineticEnergy, yname=$Kinetic\ energy$, legendlocate=topright, name=$DynELA\ Kinetic\ Energy$, ke.plot, name=$Abaqus\ Kinetic\ Energy$, Abaqus/Taylor_kineticEnergy.plot

# Height history
height, yname=$Height\ H_f\ (mm)$, name=$DynELA\ H_f$, height.plot, name=$Abaqus\ H_f$, Abaqus/Taylor_height.plot

# Radius history
radius, yname=$Radius\ R_f\ (mm)$, name=$DynELA\ R_f$, legendlocate=bottomright, radius.plot, name=$Abaqus\ R_f$, Abaqus/Taylor_radius.plot
