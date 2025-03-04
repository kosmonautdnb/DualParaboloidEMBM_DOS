# DualParaboloidEMBM_DOS
A WatcomC++ dos version of dual paraboloid environment maped bump mapping (EMBM)

Currently it's just paraboloid the "dual" term is currently misleading.

To fix this, make p.x/(p.z+2),p.y/(p.z+2) to p.x/(p.z+1),p.y/(p.z+1) and invert p on p.z < 0.
