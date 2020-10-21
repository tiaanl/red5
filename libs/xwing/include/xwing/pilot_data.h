#pragma once

enum class Rank : U8 {
  Cadet = 0x00,
  FlightOfficer = 0x01,
  Rank3 = 0x02,
  Rank4 = 0x03,
  Rank5 = 0x04,
  General = 0x05,
};

#define RANK_CADET 0x00

#define RANK_GENERAL 0x05

struct PilotData {
  U8 normalHealth;
  U8 captured;
  U8 killed;
  Rank rank;
  U32 tourOfDutyScore;
  U16 rookieNumber;
  U8 corellianCross;
  U8 mantooineMedallion;
  U8 starOfAlderaan;
  U8 pad1[32];
  U32 mazeScoreXWing;
  U32 mazeScoreYWing;
  U32 mazeScoreAWing;
};
