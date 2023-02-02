//
// Created by catslashbin on 22-11-28.
//

#ifndef CYGNOIDES_DECISION_CONSTS_H
#define CYGNOIDES_DECISION_CONSTS_H

#define SMALL_ARMOR_WIDTH 135.0f
#define SMALL_ARMOR_HEIGHT 125.0f
#define BIG_ARMOR_WIDTH 230.0f
#define BIG_ARMOR_HEIGHT 127.0f

#define MAX_FACILITY_ID 13
enum FacilityID {
    UNKNOWN = 0,

    HERO_1 = 1,

    ENGINEER_2 = 2,
    STANDARD_3 = 3,

    STANDARD_4 = 4,
    STANDARD_5 = 5,
    SENTRY_7 = 7,

    BASE_8 = 8,
    OUTPOST_10 = 10,

    UNKNOWN_BOT = 11,
    UNKNOWN_STRUCTURE = 12
};

enum ArmorType {
    SMALL_ARMOR = 0,
    BIG_ARMOR = 1
};

static ArmorType getArmorTypeFormID(FacilityID id) {
    if (id == HERO_1 || id == ENGINEER_2
        || id == STANDARD_3 || id == STANDARD_4 || id == STANDARD_5
        || id == SENTRY_7 || id == UNKNOWN_BOT)
        return SMALL_ARMOR;
    else return BIG_ARMOR;
}

#endif //CYGNOIDES_DECISION_CONSTS_H
