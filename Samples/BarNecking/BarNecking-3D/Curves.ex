# Global parameters
Parameters, xrange=7, xname=$Elongation\ of\ the\ end\ of\ the\ bar\ (mm)$, marksnumber=15, title=$Bar\ Necking\ Benchmark\ Test$, crop=True

# Temperature curve
Temperature, yname=$Temperature\ (^{\circ}C)$, legendlocate=bottomright, name=$DynELA\ T$, temperature.plot, name=$Abaqus\ T$, Abaqus/BarNecking_Temperature.plot

# Plastic strain curve
Peeq, yname=$Equivalent\ plastic\ strain\ \overline{\varepsilon}^{p}$, legendlocate=bottomright, name=$DynELA\ \overline{\varepsilon}^{p}$, plasticStrain.plot, name=$Abaqus\ \overline{\varepsilon}^{p}$, Abaqus/BarNecking_PlasticStrain.plot

# von Mises equivalent stress curve
Mises, yname=$von\ Mises\ stress\ \overline{\sigma}\ (MPa)$, legendlocate=topright, name=$DynELA\ \overline{\sigma}$, vonMises.plot, name=$Abaqus\ \overline{\sigma}$, Abaqus/BarNecking_vonMises.plot

# TimeStep curve
TimeStep, yname=$Time\ increment\ \Delta t\ (s)$, legendlocate=topright, name=$DynELA\ \Delta t$, dt.plot, name=$Abaqus\ \Delta t$, Abaqus/BarNecking_timeStep.plot

# Kinetic energy curve
KineticEnergy, yname=$Kinetic\ energy$, legendlocate=bottomright, name=$DynELA\ Kinetic\ Energy$, ke.plot, name=$Abaqus\ Kinetic\ Energy$, Abaqus/BarNecking_kineticEnergy.plot
