#if defined _WHATNOT_INCLUDE_INCLUDED
    #endinput
#endif
#define _WHATNOT_INCLUDE_INCLUDED

            // Includes
#include <a_samp>          
#include <streamer>
#include <YSI_Data\y_iterate>  
#include <YSI_Coding\y_hooks>
    
            // Definers
#define MAX_INTERACT_PICKUPS    (500)
#define INTERACT_DISTANCE       (2.0)

enum E_PICKUP_INTERACT {
    E_PICKUP_INTERACT_PICKUPID,
    E_PICKUP_INTERACT_AREAID,
    E_PICKUP_INTERACT_KEY[8 + 1],
    E_PICKUP_INTERACT_COLOR[8 + 1]
};

static 
    e_PickupInteract[MAX_INTERACT_PICKUPS][E_PICKUP_INTERACT],
    bool:gPlayerInInteractArea[MAX_PLAYERS],
    PlayerText:gInteractTD[MAX_PLAYERS][9]
;

            // Iterator
new 
    Iterator:interactIndex<MAX_INTERACT_PICKUPS>
;


hook OnPlayerDisconnect(playerid, reason) {

    Whatnot_DestroyPlayerTDs(playerid);
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerEnterDynArea(playerid, STREAMER_TAG_AREA:areaid) {

    new index = Streamer_GetIntData(STREAMER_TYPE_AREA, _:areaid, E_STREAMER_EXTRA_ID) - 1;

    if (Iter_Contains(interactIndex, index)) {
        if (e_PickupInteract[index][E_PICKUP_INTERACT_AREAID] == _:areaid) 
            Whatnot_CreatePlayerTDs(playerid, e_PickupInteract[index][E_PICKUP_INTERACT_KEY], e_PickupInteract[index][E_PICKUP_INTERACT_COLOR]);
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerLeaveDynArea(playerid, STREAMER_TAG_AREA:areaid) {
    
    new index = Streamer_GetIntData(STREAMER_TYPE_AREA, _:areaid, E_STREAMER_EXTRA_ID) - 1;

    if (Iter_Contains(interactIndex, index)) {
        if (e_PickupInteract[index][E_PICKUP_INTERACT_AREAID] == _:areaid) 
            Whatnot_DestroyPlayerTDs(playerid);
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}



/*
                    .oooooo..o ooooooooooooo   .oooooo.     .oooooo.   oooo    oooo  .oooooo..o 
                    d8P'    `Y8 8'   888   `8  d8P'  `Y8b   d8P'  `Y8b  `888   .8P'  d8P'    `Y8 
                    Y88bo.          888      888      888 888          888  d8'    Y88bo.      
                     `"Y8888o.      888      888      888 888          88888[       `"Y8888o.  
                         `"Y88b     888      888      888 888          888`88b.         `"Y88b 
                    oo     .d8P     888      `88b    d88' `88b    ooo  888  `88b.  oo     .d8P 
                    8""88888P'     o888o      `Y8bood8P'   `Y8bood8P'  o888o  o888o 8""88888P'  
*/


stock AddPickupInteract(modelid, Float:x, Float:y, Float:z, const key[], const color[] = "#DF5454", Float:distance = INTERACT_DISTANCE, worldid = -1, interiorid = -1) {
    
    new index;

    if((index = Iter_Alloc(interactIndex)) == ITER_NONE) {
        print("[Whatnot] Error: Maximum limit of interactive pickups reached.");
        return INVALID_STREAMER_ID;
    }

    new pickup_id = CreateDynamicPickup(
        .modelid = modelid, 
        .type = 1, 
        .x = x, 
        .y = y, 
        .z = z, 
        .worldid = worldid, 
        .interiorid = interiorid, 
        .playerid = -1
    );
    
    if(pickup_id == INVALID_STREAMER_ID) {
        print("[Whatnot] Error: Failed to create dynamic pickup.");
        return INVALID_STREAMER_ID;
    }

    new area_id = CreateDynamicSphere(
        .x = x, 
        .y = y, 
        .z = z, 
        .size = distance, 
        .worldid = worldid, 
        .interiorid = interiorid, 
        .playerid = -1
    );
    
    Streamer_SetIntData(STREAMER_TYPE_AREA, area_id, E_STREAMER_EXTRA_ID, index + 1);

    e_PickupInteract[index][E_PICKUP_INTERACT_PICKUPID] = pickup_id;
    e_PickupInteract[index][E_PICKUP_INTERACT_AREAID]   = area_id;
    format(e_PickupInteract[index][E_PICKUP_INTERACT_KEY], 8, "%s", key);
    format(e_PickupInteract[index][E_PICKUP_INTERACT_COLOR], 8, "%s", color);

    return pickup_id;
}

stock DestroyPickupInteract(index)  {

    if(!Iter_Contains(interactIndex, index))
        return 0;

    DestroyDynamicPickup(e_PickupInteract[index][E_PICKUP_INTERACT_PICKUPID]);
    DestroyDynamicArea(e_PickupInteract[index][E_PICKUP_INTERACT_AREAID]);

    Iter_Remove(interactIndex, index);
    return 1;
}

static stock bool:IsPlayerInAnyInteractArea(playerid)
    return gPlayerInInteractArea[playerid];

static stock Whatnot_CreatePlayerTDs(playerid, const key_name[], const color[]) {
    
    if (IsPlayerInAnyInteractArea(playerid))
        return;
    
    gInteractTD[playerid][0] = CreatePlayerTextDraw(playerid, 297.000, 406.968, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, gInteractTD[playerid][0], 9.989, 17.000);
    PlayerTextDrawAlignment(playerid, gInteractTD[playerid][0], 1);
    PlayerTextDrawColor(playerid, gInteractTD[playerid][0], Whatnot_HexToColor(color));
    PlayerTextDrawSetShadow(playerid, gInteractTD[playerid][0], 0);
    PlayerTextDrawSetOutline(playerid, gInteractTD[playerid][0], 0);
    PlayerTextDrawBackgroundColor(playerid, gInteractTD[playerid][0], 255);
    PlayerTextDrawFont(playerid, gInteractTD[playerid][0], 4);
    PlayerTextDrawSetProportional(playerid, gInteractTD[playerid][0], 1);
    
    gInteractTD[playerid][1] = CreatePlayerTextDraw(playerid, 295.000, 409.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, gInteractTD[playerid][1], 13.628, 12.430);
    PlayerTextDrawAlignment(playerid, gInteractTD[playerid][1], 1);
    PlayerTextDrawColor(playerid, gInteractTD[playerid][1], Whatnot_HexToColor(color));
    PlayerTextDrawSetShadow(playerid, gInteractTD[playerid][1], 0);
    PlayerTextDrawSetOutline(playerid, gInteractTD[playerid][1], 0);
    PlayerTextDrawBackgroundColor(playerid, gInteractTD[playerid][1], 255);
    PlayerTextDrawFont(playerid, gInteractTD[playerid][1], 4);
    PlayerTextDrawSetProportional(playerid, gInteractTD[playerid][1], 1);
    
    gInteractTD[playerid][2] = CreatePlayerTextDraw(playerid, 294.000, 417.589, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, gInteractTD[playerid][2], 7.000, 7.778);
    PlayerTextDrawAlignment(playerid, gInteractTD[playerid][2], 1);
    PlayerTextDrawColor(playerid, gInteractTD[playerid][2], Whatnot_HexToColor(color));
    PlayerTextDrawSetShadow(playerid, gInteractTD[playerid][2], 0);
    PlayerTextDrawSetOutline(playerid, gInteractTD[playerid][2], 0);
    PlayerTextDrawBackgroundColor(playerid, gInteractTD[playerid][2], 255);
    PlayerTextDrawFont(playerid, gInteractTD[playerid][2], 4);
    PlayerTextDrawSetProportional(playerid, gInteractTD[playerid][2], 1);
    
    gInteractTD[playerid][3] = CreatePlayerTextDraw(playerid, 294.028, 405.600, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, gInteractTD[playerid][3], 7.000, 7.776);
    PlayerTextDrawAlignment(playerid, gInteractTD[playerid][3], 1);
    PlayerTextDrawColor(playerid, gInteractTD[playerid][3], Whatnot_HexToColor(color));
    PlayerTextDrawSetShadow(playerid, gInteractTD[playerid][3], 0);
    PlayerTextDrawSetOutline(playerid, gInteractTD[playerid][3], 0);
    PlayerTextDrawBackgroundColor(playerid, gInteractTD[playerid][3], 255);
    PlayerTextDrawFont(playerid, gInteractTD[playerid][3], 4);
    PlayerTextDrawSetProportional(playerid, gInteractTD[playerid][3], 1);
    
    gInteractTD[playerid][4] = CreatePlayerTextDraw(playerid, 298.219, 408.968, key_name);
    PlayerTextDrawLetterSize(playerid, gInteractTD[playerid][4], 0.309, 1.350);
    PlayerTextDrawAlignment(playerid, gInteractTD[playerid][4], 1);
    PlayerTextDrawColor(playerid, gInteractTD[playerid][4], -1);
    PlayerTextDrawSetShadow(playerid, gInteractTD[playerid][4], 1);
    PlayerTextDrawSetOutline(playerid, gInteractTD[playerid][4], 1);
    PlayerTextDrawBackgroundColor(playerid, gInteractTD[playerid][4], 0);
    PlayerTextDrawFont(playerid, gInteractTD[playerid][4], 1);
    PlayerTextDrawSetProportional(playerid, gInteractTD[playerid][4], 1);
    
    gInteractTD[playerid][5] = CreatePlayerTextDraw(playerid, 311.230, 407.557, "press_for");
    PlayerTextDrawLetterSize(playerid, gInteractTD[playerid][5], 0.109, 0.710);
    PlayerTextDrawAlignment(playerid, gInteractTD[playerid][5], 1);
    PlayerTextDrawColor(playerid, gInteractTD[playerid][5], -1448498689);
    PlayerTextDrawSetShadow(playerid, gInteractTD[playerid][5], 1);
    PlayerTextDrawSetOutline(playerid, gInteractTD[playerid][5], 1);
    PlayerTextDrawBackgroundColor(playerid, gInteractTD[playerid][5], 0);
    PlayerTextDrawFont(playerid, gInteractTD[playerid][5], 1);
    PlayerTextDrawSetProportional(playerid, gInteractTD[playerid][5], 1);
    
    gInteractTD[playerid][6] = CreatePlayerTextDraw(playerid, 302.588, 405.709, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, gInteractTD[playerid][6], 7.000, 7.776);
    PlayerTextDrawAlignment(playerid, gInteractTD[playerid][6], 1);
    PlayerTextDrawColor(playerid, gInteractTD[playerid][6], Whatnot_HexToColor(color));
    PlayerTextDrawSetShadow(playerid, gInteractTD[playerid][6], 0);
    PlayerTextDrawSetOutline(playerid, gInteractTD[playerid][6], 0);
    PlayerTextDrawBackgroundColor(playerid, gInteractTD[playerid][6], 255);
    PlayerTextDrawFont(playerid, gInteractTD[playerid][6], 4);
    PlayerTextDrawSetProportional(playerid, gInteractTD[playerid][6], 1);
    
    gInteractTD[playerid][7] = CreatePlayerTextDraw(playerid, 302.588, 417.489, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, gInteractTD[playerid][7], 7.000, 7.776);
    PlayerTextDrawAlignment(playerid, gInteractTD[playerid][7], 1);
    PlayerTextDrawColor(playerid, gInteractTD[playerid][7], Whatnot_HexToColor(color));
    PlayerTextDrawSetShadow(playerid, gInteractTD[playerid][7], 0);
    PlayerTextDrawSetOutline(playerid, gInteractTD[playerid][7], 0);
    PlayerTextDrawBackgroundColor(playerid, gInteractTD[playerid][7], 255);
    PlayerTextDrawFont(playerid, gInteractTD[playerid][7], 4);
    PlayerTextDrawSetProportional(playerid, gInteractTD[playerid][7], 1);
    
    gInteractTD[playerid][8] = CreatePlayerTextDraw(playerid, 311.230, 413.858, "interagir");
    PlayerTextDrawLetterSize(playerid, gInteractTD[playerid][8], 0.109, 0.710);
    PlayerTextDrawAlignment(playerid, gInteractTD[playerid][8], 1);
    PlayerTextDrawColor(playerid, gInteractTD[playerid][8], -1448498689);
    PlayerTextDrawSetShadow(playerid, gInteractTD[playerid][8], 1);
    PlayerTextDrawSetOutline(playerid, gInteractTD[playerid][8], 1);
    PlayerTextDrawBackgroundColor(playerid, gInteractTD[playerid][8], 0);
    PlayerTextDrawFont(playerid, gInteractTD[playerid][8], 4);
    PlayerTextDrawSetProportional(playerid, gInteractTD[playerid][8], 1);

    for (new i; i < sizeof gInteractTD[]; i++)
        PlayerTextDrawShow(playerid, gInteractTD[playerid][i]);

    gPlayerInInteractArea[playerid] = true;
}

static stock Whatnot_DestroyPlayerTDs(playerid)  {
    
    if (!IsPlayerInAnyInteractArea(playerid))
        return;

    for (new i; i < sizeof gInteractTD[]; i++) {
        PlayerTextDrawDestroy(playerid, gInteractTD[playerid][i]);
        gInteractTD[playerid][i] = PlayerText:INVALID_PLAYER_TEXT_DRAW;
    }

    gPlayerInInteractArea[playerid] = false;
}

static stock Whatnot_HexToColor(const color[]) {

    new hex;

    sscanf(color[1], "h", hex);

    return (hex << 8) | 0xFF;
}