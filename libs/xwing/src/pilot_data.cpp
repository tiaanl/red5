#pragma once

#if 0
Hex Offset:     Hex Value:      Description:
02              00              Normal Health
01                              Captured
02                              Killed
03                              From 00 to 05   From Cadet to General in rank
04-07           Any             Tour of duty score
08-09           Any             Rookie number
0A              01              Corellian Cross
0B              01              Mantooine Medallion
0C              01              Star of Alderaan
26-29           Any             Maze score for X-Wing
2A-2D           Any             Maze score for Y-Wing
2E-31           Any             Maze score for A-Wing
86              From 00 to 08   Maze level completed X-Wing
87              From 00 to 08   Maze level completed Y-Wing
88              From 00 to 08   Maze level completed A-Wing
A0-A3           Any             X-Wing score for History mission 1
A4-B7           Any             X-Wing score for rest of History missions
E0-F7           Any             Y-Wing score for History missions 1-6
120-137         Any             A-Wing score for History missions 1-6
220-225         Place 01 in
                position to
                complete
                mission         History missions 1-6 complete for X-Wing
230-235         01              History missions 1-6 complete for Y-Wing
240-245         01              History missions 1-6 complete for A-Wing
2E0-2E2         00=Not active   Tour of Duty missions 1-3
                01=Active
2F0             From 00 to 0B   No of missions complete for Tour 1 (1-12)
2F1             From 00 to 0B   No of missions complete for Tour 2 (1-12)
2F2             From 00 to 0C   No of missions complete for Tour 3 (1-13)
2F8-3FB         Any             Score for Tour 1 - mission 1
2FC-327         Any             Score for Tour 1 - missions 2-12
25C-38B         Any             Score for Tour 2 - missions 1-12
3C0-3F3         Any             Score for Tour 3 - missions 1-13
#endif  // 0


