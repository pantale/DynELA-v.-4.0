# Global parameters
Parameters, xrange=7, xname=$Elongation\ of\ the\ end\ of\ the\ bar\ (mm)$, marksnumber=15, title=$Bar\ Necking\ Benchmark\ Test$, crop=True

# Temperature curve
temperature, yname=$Temperature\ (^{\circ}C)$, legendlocate=bottomright, name=$DynELA\ T$, temperature.plot, name=$Abaqus\ T$, Abaqus/BarNecking_Temperature.plot

# Plastic strain curve
plasticStrain, yname=$Equivalent\ plastic\ strain\ \overline{\varepsilon}^{p}$, legendlocate=bottomright, name=$DynELA\ \overline{\varepsilon}^{p}$, plasticStrain.plot, name=$Abaqus\ \overline{\varepsilon}^{p}$, Abaqus/BarNecking_PlasticStrain.plot

# von Mises equivalent stress curve
vonMises, yname=$von\ Mises\ stress\ \overline{\sigma}\ (MPa)$, legendlocate=topright, name=$DynELA\ \overline{\sigma}$, vonMises.plot, name=$Abaqus\ \overline{\sigma}$, Abaqus/BarNecking_vonMises.plot

# TimeStep curve
timeStep, yname=$Time\ increment\ \Delta t\ (s)$, legendlocate=topright, name=$DynELA\ \Delta t$, dt.plot, name=$Abaqus\ \Delta t$, Abaqus/BarNecking_timeStep.plot

# Kinetic energy curve
kineticEnergy, yname=$Kinetic\ energy$, legendlocate=bottomright, name=$DynELA\ Kinetic\ Energy$, ke.plot, name=$Abaqus\ Kinetic\ Energy$, Abaqus/BarNecking_kineticEnergy.plot

# Radius history
radius, yname=$Radius\ R_f\ (mm)$, name=$DynELA\ R_f$, legendlocate=topright, radius.plot, name=$Abaqus\ R_f$, Abaqus/BarNecking_radius.plot
