#include <string.h>
#include "setcustomcallbacks.h"
#include "legoland.h"
#include "ride_interfaces.h"

// FUNCTION: LEGOLAND 0x00452c20
LEGO_EXPORT void SetCustomCallbacks(struct ClassNode *head) {
    struct CallbackTable *iface = head->iface;

    if (_stricmp(head->name, "PATH CONTROL") == 0) {
        iface->cb_98 = AddBasicPath;
        iface->cb_9c = RemoveBasicPath;
        iface->cb_a0 = DrawBasicPath;
    }
    // STRING: LEGOLAND 0x004b8a64
    else if (_stricmp(head->name, "FOUNTAIN 1") == 0 ||
             // STRING: LEGOLAND 0x004b8a58
             _stricmp(head->name, "FOUNTAIN 2") == 0 ||
             // STRING: LEGOLAND 0x004b8a4c
             _stricmp(head->name, "FOUNTAIN 3") == 0) {
        iface->cb_98 = FUN_004529e0;
        iface->cb_9c = RemoveSoundObject;
        iface->cb_ac = FUN_004529c0;
        FUN_00452990(head);
    }
    // STRING: LEGOLAND 0x004b8a34
    else if (_stricmp(head->name, "crystal power station") == 0) {
        iface->cb_98 = FUN_00452b20;
        iface->cb_9c = RemoveSoundObject;
        iface->cb_ac = FUN_00452ab0;
        FUN_00452a80(head);
    }
    // STRING: LEGOLAND 0x004b8a20
    else if (_stricmp(head->name, "small power station") == 0) {
        iface->cb_98 = FUN_00452ad0;
        iface->cb_9c = RemoveSoundObject;
        iface->cb_ac = FUN_00452ab0;
        FUN_00452a80(head);
    }
    // STRING: LEGOLAND 0x004b8a14
    else if (_stricmp(head->name, "Dino Big") == 0 ||
             // STRING: LEGOLAND 0x004b8a08
             _stricmp(head->name, "Dino Small") == 0 ||
             // STRING: LEGOLAND 0x004b89fc
             _stricmp(head->name, "Dino Mini") == 0) {
        iface->cb_98 = FUN_00452bc0;
        iface->cb_9c = RemoveSoundObject;
        iface->cb_ac = FUN_00452ba0;
        FUN_00452b70(head);
    }
    // STRING: LEGOLAND 0x004b89e4
    else if (_stricmp("DRIVING SCHOOL PUMPS", head->name) == 0) {
        iface->cb_a4 = FUN_00411a10;
        iface->cb_8c = FUN_00411a20;
        iface->cb_90 = FUN_00411cd0;
        iface->cb_98 = FUN_00411bf0;
        iface->cb_9c = FUN_00411c70;
    }
    else if (_stricmp("DRIVING SCHOOL", head->name) == 0) {
        iface->cb_a4 = FUN_00405370;
        iface->cb_8c = FUN_00405570;
        iface->cb_90 = FUN_00405740;
        iface->cb_94 = FUN_004058a0;
        iface->cb_98 = FUN_00405630;
        iface->cb_9c = FUN_00405940;
        iface->cb_a8 = FUN_00405bd0;
        iface->cb_a0 = FUN_00405ad0;
        iface->cb_b0 = FUN_00405b10;
        iface->cb_bc = FUN_00405e70;
        iface->cb_b8 = FUN_00406070;
        iface->cb_ac = FUN_00405460;
        iface->cb_c0 = FUN_00406050;
    }
    // STRING: LEGOLAND 0x004b89cc
    else if (_stricmp("DRIVING SCHOOL ROADS", head->name) == 0) {
        iface->cb_a4 = FUN_00413a10;
        iface->cb_ac = FUN_00413a80;
        iface->cb_8c = FUN_00413ad0;
        iface->cb_90 = FUN_00413b50;
        iface->cb_94 = FUN_00413fa0;
        iface->cb_98 = FUN_00414020;
        iface->cb_9c = FUN_00414220;
    }
    // STRING: LEGOLAND 0x004b89bc
    else if (_stricmp("ZEBRA CROSSING", head->name) == 0) {
        iface->cb_a4 = FUN_00414940;
        iface->cb_8c = FUN_00414830;
        iface->cb_90 = FUN_00414880;
        iface->cb_94 = FUN_00413fa0;
        iface->cb_98 = FUN_00414950;
        iface->cb_9c = FUN_00414220;
    }
    else if (_stricmp("ENTRANCE 1", head->name) == 0) {
        iface->cb_a4 = FUN_0042de50;
        iface->cb_ac = FUN_0042def0;
        iface->cb_a8 = FUN_0042dfa0;
        iface->cb_b0 = FUN_0042d9c0;
        iface->cb_9c = FUN_0042df70;
    }
    else if (_stricmp("POTTING SHED", head->name) == 0) {
        iface->cb_a4 = FUN_0043ce60;
        iface->cb_8c = FUN_0043d1d0;
        iface->cb_98 = FUN_0043ceb0;
        iface->cb_9c = FUN_0043ced0;
        iface->cb_a8 = FUN_0043cf00;
        iface->cb_b0 = FUN_0043d0b0;
        iface->cb_ac = FUN_0043d1c0;
        iface->cb_a0 = FUN_0043d210;
    }
    else if (_stricmp("MECHANICS HUT", head->name) == 0) {
        iface->cb_a4 = FUN_0043d250;
        iface->cb_8c = FUN_0043d740;
        iface->cb_98 = FUN_0043d2a0;
        iface->cb_9c = FUN_0043d2c0;
        iface->cb_a8 = FUN_0043d2f0;
        iface->cb_b0 = FUN_0043d580;
        iface->cb_ac = FUN_0043d730;
        iface->cb_a0 = FUN_0043d780;
    }
    // STRING: LEGOLAND 0x004b8990
    else if (_stricmp("CAROUSEL", head->name) == 0) {
        iface->cb_a4 = FUN_0042c280;
        iface->cb_ac = FUN_0042c3f0;
        iface->cb_8c = FUN_0042c460;
        iface->cb_a8 = FUN_0042c820;
        iface->cb_b0 = FUN_0042bcf0;
        iface->cb_9c = FUN_0042c4a0;
        iface->cb_98 = FUN_0042c520;
        iface->cb_a0 = FUN_0042c550;
        iface->cb_b8 = FUN_0042c600;
        iface->cb_bc = FUN_0042c590;
    }
    // STRING: LEGOLAND 0x004b8984
    else if (_stricmp("BALLOONZ", head->name) == 0) {
        iface->cb_a4 = FUN_0042a7b0;
        iface->cb_8c = FUN_0042ba40;
        iface->cb_98 = FUN_0042a950;
        iface->cb_9c = FUN_0042aa10;
        iface->cb_a8 = FUN_0042aa90;
        iface->cb_b0 = FUN_0042b2e0;
        iface->cb_ac = FUN_0042b9d0;
        iface->cb_a0 = FUN_0042b2a0;
        iface->cb_bc = FUN_0042ba80;
        iface->cb_b8 = FUN_0042baf0;
    }
    // STRING: LEGOLAND 0x004b8970
    else if (_stricmp("EARTH SLIDE RIDE", head->name) == 0) {
        iface->cb_a4 = FUN_0042d100;
        iface->cb_ac = FUN_0042d1f0;
        iface->cb_8c = FUN_0042d230;
        iface->cb_a8 = FUN_0042d610;
        iface->cb_b0 = FUN_0042d070;
        iface->cb_9c = FUN_0042d270;
        iface->cb_98 = FUN_0042d2c0;
        iface->cb_bc = FUN_0042d2f0;
        iface->cb_b8 = FUN_0042d400;
    }
    // STRING: LEGOLAND 0x004b8964
    else if (_stricmp("CASTLE BBQ", head->name) == 0) {
        iface->cb_a4 = FUN_0042e870;
        iface->cb_ac = FUN_0042e8b0;
        iface->cb_8c = FUN_0042e8d0;
        iface->cb_a8 = FUN_0042ea60;
        iface->cb_b0 = FUN_0042e910;
        iface->cb_98 = FUN_0042e9c0;
        iface->cb_9c = FUN_0042ea10;
    }
    // STRING: LEGOLAND 0x004b8954
    else if (_stricmp("FOODCART DRINK", head->name) == 0) {
        iface->cb_a4 = FUN_0042e770;
        iface->cb_ac = FUN_0042e7a0;
        iface->cb_8c = FUN_0042e8d0;
        iface->cb_a8 = FUN_0042ec10;
        iface->cb_9c = FUN_004312c0;
        iface->cb_b0 = FUN_0042e830;
    }
    // STRING: LEGOLAND 0x004b8944
    else if (_stricmp("FOODCART FOOD", head->name) == 0) {
        iface->cb_a4 = FUN_0042e7f0;
        iface->cb_ac = FUN_0042e820;
        iface->cb_8c = FUN_0042e8d0;
        iface->cb_a8 = FUN_0042ed70;
        iface->cb_9c = FUN_004312c0;
        iface->cb_b0 = FUN_0042e830;
    }
    // STRING: LEGOLAND 0x004b8930
    else if (_stricmp("FOODCART ICECREAM", head->name) == 0) {
        iface->cb_a4 = FUN_0042e7b0;
        iface->cb_ac = FUN_0042e7e0;
        iface->cb_8c = FUN_0042e8d0;
        iface->cb_a8 = FUN_00431170;
        iface->cb_9c = FUN_004312c0;
        iface->cb_b0 = FUN_0042e830;
    }
    // STRING: LEGOLAND 0x004b8920
    else if (_stricmp("OCTOPUS CAFE", head->name) == 0) {
        iface->cb_a4 = FUN_00431300;
        iface->cb_98 = FUN_004314f0;
        iface->cb_8c = FUN_0042e8d0;
        iface->cb_a8 = FUN_004316f0;
        iface->cb_9c = FUN_004312c0;
        iface->cb_b0 = FUN_00431d00;
        iface->cb_ac = FUN_00431520;
    }
    // STRING: LEGOLAND 0x004b8910
    else if (_stricmp("RESTAURANT 1", head->name) == 0) {
        iface->cb_a4 = FUN_0042f030;
        iface->cb_8c = FUN_0042e8d0;
        iface->cb_a8 = FUN_0042f1a0;
        iface->cb_98 = FUN_0042ef10;
        iface->cb_9c = FUN_0042efb0;
        iface->cb_ac = FUN_0042f720;
        iface->cb_b0 = FUN_0042f4c0;
        iface->cb_bc = FUN_004322a0;
        iface->cb_b8 = FUN_00432310;
    }
    // STRING: LEGOLAND 0x004b8900
    else if (_stricmp("RESTAURANT 2", head->name) == 0) {
        iface->cb_a4 = FUN_0042f770;
        iface->cb_8c = FUN_0042e8d0;
        iface->cb_a8 = FUN_0042fbb0;
        iface->cb_98 = FUN_0042f9a0;
        iface->cb_9c = FUN_0042fa40;
        iface->cb_a0 = FUN_004304a0;
        iface->cb_ac = FUN_00431120;
        iface->cb_bc = FUN_00432390;
        iface->cb_b8 = FUN_00432400;
        iface->cb_b0 = FUN_00430b10;
    }
    // STRING: LEGOLAND 0x004b88f4
    else if (_stricmp("CHUCK WAGON", head->name) == 0) {
        iface->cb_a4 = FUN_0042e220;
        iface->cb_8c = FUN_0042e8d0;
        iface->cb_9c = FUN_004312c0;
        iface->cb_a8 = FUN_0042e2a0;
        iface->cb_b0 = FUN_0042e260;
        iface->cb_ac = FUN_0042e250;
    }
    // STRING: LEGOLAND 0x004b88e8
    else if (_stricmp("SHARK CAFE", head->name) == 0) {
        iface->cb_a4 = FUN_0042e5d0;
        iface->cb_ac = FUN_0042e600;
        iface->cb_8c = FUN_0042e8d0;
        iface->cb_9c = FUN_004312c0;
        iface->cb_a8 = FUN_0042e610;
        iface->cb_b0 = FUN_0042e830;
    }
    // STRING: LEGOLAND 0x004b83dc
    else if (_stricmp("SHARK CAFE BROLLY", head->name) == 0) {
        iface->cb_a4 = FUN_0042e460;
        iface->cb_8c = FUN_0042e4c0;
        iface->cb_98 = FUN_0042e500;
        iface->cb_a0 = FUN_0042e560;
        iface->cb_ac = FUN_0042e4b0;
    }
    // STRING: LEGOLAND 0x004b537c
    else if (_stricmp("BOATING SCHOOL WATER", head->name) == 0) {
        iface->cb_a4 = FUN_0041b830;
        iface->cb_8c = FUN_0041b880;
        iface->cb_90 = FUN_0041bd40;
        iface->cb_94 = FUN_0041bfb0;
        iface->cb_98 = FUN_0041b8e0;
        iface->cb_9c = FUN_0041c130;
    }
    else if (_stricmp("BOATING SCHOOL", head->name) == 0) {
        iface->cb_a4 = FUN_00419d10;
        iface->cb_ac = FUN_00419ef0;
        iface->cb_8c = FUN_0041a000;
        iface->cb_90 = FUN_0041a2f0;
        iface->cb_94 = FUN_0041a3d0;
        iface->cb_98 = FUN_0041a040;
        iface->cb_9c = FUN_0041a530;
        iface->cb_a8 = FUN_0041a720;
        iface->cb_b0 = FUN_0041abd0;
        iface->cb_bc = FUN_0041acf0;
        iface->cb_b8 = FUN_0041aee0;
        iface->cb_c0 = FUN_0041b100;
    }
    // STRING: LEGOLAND 0x004b5354
    else if (_stricmp("BOATING SCHOOL MERMAID", head->name) == 0) {
        iface->cb_a4 = FUN_0041b250;
        iface->cb_8c = FUN_0041b260;
        iface->cb_90 = FUN_0041b4c0;
        iface->cb_94 = FUN_0041b6d0;
        iface->cb_98 = FUN_0041b2a0;
        iface->cb_9c = FUN_0041b6f0;
    }
    // STRING: LEGOLAND 0x004b88d4
    else if (_stricmp("JUNGLE CRUISE WATER", head->name) == 0) {
        iface->cb_a4 = FUN_00436190;
        iface->cb_8c = FUN_004361a0;
        iface->cb_90 = FUN_00436200;
        iface->cb_94 = FUN_00436470;
        iface->cb_98 = FUN_004365f0;
        iface->cb_9c = FUN_00436a40;
    }
    else if (_stricmp("JUNGLE CRUISE", head->name) == 0) {
        iface->cb_a4 = FUN_00434cb0;
        iface->cb_ac = FUN_00434e50;
        iface->cb_8c = FUN_00434f50;
        iface->cb_90 = FUN_00435150;
        iface->cb_94 = FUN_00435230;
        iface->cb_98 = FUN_00434f90;
        iface->cb_9c = FUN_00435470;
        iface->cb_a8 = FUN_00435750;
        iface->cb_b0 = FUN_00435bd0;
        iface->cb_bc = FUN_00435c70;
        iface->cb_b8 = FUN_00435ec0;
        iface->cb_c0 = FUN_00436160;
    }
    // STRING: LEGOLAND 0x004b88b8
    else if (_stricmp("JUNGLE CRUISE MONKEY TREE", head->name) == 0) {
        iface->cb_a4 = FUN_00433ca0;
        iface->cb_ac = FUN_00433cd0;
        iface->cb_8c = FUN_00433ce0;
        iface->cb_90 = FUN_00433d90;
        iface->cb_94 = FUN_00433fa0;
        iface->cb_98 = FUN_00433d20;
        iface->cb_9c = FUN_00433fc0;
        iface->cb_a0 = FUN_00434040;
    }
    // STRING: LEGOLAND 0x004b889c
    else if (_stricmp("JUNGLE CRUISE MONKEY FISH", head->name) == 0) {
        iface->cb_a4 = FUN_00434080;
        iface->cb_ac = FUN_004340b0;
        iface->cb_8c = FUN_004340c0;
        iface->cb_90 = FUN_00434330;
        iface->cb_94 = FUN_00434650;
        iface->cb_98 = FUN_00434100;
        iface->cb_9c = FUN_00434670;
        iface->cb_a0 = FUN_00434740;
    }

    FUN_00403080(head, iface);
    FUN_00410d60(head, iface);
    FUN_00405110(head, iface);
    FUN_004068b0(head, iface);
    FUN_004078f0(head, iface);
    FUN_00416e50(head, iface);
    FUN_00403bb0(head, iface);
    Joust_GetInterfaces(head, iface);
    TempleSlide_GetInterfaces(head, iface);
    SpiderRide(head, iface);
    FUN_00415030(head, iface);
    FUN_00418c80(head, iface);
    FUN_004329c0(head, iface);
    FUN_0043a400(head, iface);
    SpaceTowerRide(head, iface);
    FUN_0043c760(head, iface);
    FUN_0043e220(head, iface);
    FUN_004254d0(head, iface);
}
