# AutoBattler
A deterministic simulation of battle between spheres!

#What's in it
- Simulation happens in Fixed Time Steps.
- An arena made of square tiles.
- Spheres are assigned to teams and are spawned on an arbitrary tile.
- Spheres move in simulation at a certain user specified tiles per Time Step.
- Attack and cooldown spans some user specified time frames.
- Spheres dim down a little bit when hit.
- If all spheres in one team dies, simulation restarts.
- Spheres have no will of their own. They are directed by Battle Subsystem. Visual representation should look the same even if commands come from server.

#Known Issues
- Spheres of same team overlap.
- Uses extremely simple path finding. Incorporating a better algorithm will also prevent sphere overlap.
