# Attempt log for unmatched functions

One file per function that is not yet at 100%, named `<address>-<name>.md`. Each says what still differs
from the original, what has already been tried (with the score it gave), and ideas nobody has tried yet.

**Before working on a function, read its file and don't repeat an attempt listed there** unless you are
changing something else at the same time. **After working on it, add your attempts** to the table, even the
ones that failed: date, who (person, or model for agents), the change, the score. Update "Best" and "What
still differs" when they change. When a function reaches 100%, delete its file.

## What applies to most of them

- **Declaration order almost never changes MSVC6's register allocation.** Nearly every agent tried
  reordering locals; it changed nothing in all but one case. Don't spend cycles on it.
- **A frame-size difference (`sub esp, N`) means a local is missing or extra.** MSVC drops locals that are
  never used, so a declared-but-unused variable doesn't count. Find the real variable behind the
  original's extra slot (look at which `[esp+...]` offsets it uses). Don't add fake padding locals.
- **The original has `push ebp; mov ebp, esp` and no asm-only instructions?** Wrap the function in
  `#pragma optimize("y", off)` / `#pragma optimize("y", on)` (`decomp-tips.md`). FUN_00485fe0 went from 33% to 77%
  with that alone.
- **Late pushes of callee-saved registers** (after an early check) usually mean the rest of the function is
  inside an `if (...) { }` block in the source, not after an early `return`.
- **`repe cmpsd` comes from `memcmp`**, `rep stosd` from `memset` with a constant size (FUN_00452030).
- **MSVC already turns a tail call to the same function into a loop**; writing the loop by hand made
  SearchJunglePathConnected worse.
- **Hand-written asm:** `tools/asm2naked.py` writes `lea reg, [ebp-x]` with a 1-byte displacement; if the
  original uses the 4-byte form, the function comes out shorter and every later jump shifts. Emit those
  instructions as raw bytes with `_emit` (RenderTransSprite).
- **reccmp false positives:** a constant that falls inside a data symbol's range in only one of the two
  images shows as a diff on a byte-identical line (`decomp-tips.md`).
- **Endpoint sums:** when a loop's final coordinate additions use different registers, calculate both
  coordinates in a `Point end` before updating the running origin. This matched `BuildQueueTable`
  (93.48 -> 100%); reversing addition operands or chaining assignments did not help.
  A paired `Point` also matched `UpdateLogFlumeEntranceEntry` (`FUN_0040be00`,
  90.61 -> 100%): calculate x and y from the global ride before passing them to the
  queue update. Caching the ride pointer, using scalar x/y locals, or introducing a
  TileId pointer did not match. All 14 alternative forms were compared before retaining
  the Point with direct global accesses; no assembly or volatile access was needed.
- **Branch-local field updates:** assigning a direction byte in each turn branch, then masking the
  field after the join, can preserve two original stores without volatile access. Updating a local
  and assigning the field twice instead let MSVC6 eliminate the first store. This matched
  `UpdateDrivingSchoolCarVelocity` (94.29 -> 100%).
- **Terminal shape-selection branches:** returning immediately after every call in a complete
  `if`/`else` tree can change MSVC6's shared argument/call tails. This matched `UpdateQueuePathShape`
  (98.43 -> 100%); a single return after the tree produced a different layout. Argument-side flag
  assignments, narrow flag casts and changing the mask type did not match.
- **Token lifetimes:** initializing a token pointer before a buffer copy and restoring a delimiter
  before advancing the token can keep the token and resolved object in separate saved registers.
  This improved `LoadObjectClassAliasElements` (73.10 -> 98.63%) without redundant increments;
  advancing immediately after lookup left a scheduling difference.
- **Rectangle intermediates:** `HTBubbleHelpWithMood` (`FUN_00455fc0`, 82.35 -> 100%) needs
  its unpadded right edge stored in the RECT before the clipping branch; add the icon allowance
  in the edge test. Correcting that state, the center comparison order and RECT initialization
  allowed all five volatile qualifiers and every permuter temporary to be removed. Qualifier
  removal and initialization changes alone did not recover the match (64 standalone trials,
  65.09-83.96% against an 83.24% harness baseline); the natural rectangle form reached 100%.

## Index

| Address | Function | File | Best |
|---|---|---|---|
| 0x00401f30 | [PickQueueTurn](0x00401f30-PickQueueTurn.md) | ride_bloke.c | 70.73% |
| 0x00402780 | [FUN_00402780](0x00402780-FUN_00402780.md) | ride_bloke.c | 75.00% |
| 0x00402dc0 | [CastleLevel1Update](0x00402dc0-CastleLevel1Update.md) | castle_level.c | 85.88% |
| 0x00404630 | [CoptersPlaceRider](0x00404630-CoptersPlaceRider.md) | copters.c | 54.39% |
| 0x00405bd0 | [DrivingSchoolUpdate](0x00405bd0-DrivingSchoolUpdate.md) | driving_school.c | 84.09% |
| 0x00407ad0 | [JoustRemoveObject](0x00407ad0-JoustRemoveObject.md) | joust.c | 95.00% |
| 0x00408f90 | [FUN_00408f90](0x00408f90-FUN_00408f90.md) | log_flume.c | 40.96% |
| 0x004092b0 | [FUN_004092b0](0x004092b0-FUN_004092b0.md) | log_flume.c | 84.62% |
| 0x0040a600 | [LogFlumeEntranceAddObject](0x0040a600-LogFlumeEntranceAddObject.md) | log_flume.c | 80.40% |
| 0x0040a930 | [LogFlumeEntranceCalcCursor](0x0040a930-LogFlumeEntranceCalcCursor.md) | log_flume.c | 67.36% |
| 0x0040abf0 | [LogFlumeEntranceRemoveObject](0x0040abf0-LogFlumeEntranceRemoveObject.md) | log_flume.c | 95.10% |
| 0x0040ae90 | [FUN_0040ae90](0x0040ae90-FUN_0040ae90.md) | log_flume.c | 45.11% |
| 0x0040bab0 | [FUN_0040bab0](0x0040bab0-FUN_0040bab0.md) | log_flume.c | 44.09% |
| 0x0040bf70 | [LogFlumeEntranceUpdate](0x0040bf70-LogFlumeEntranceUpdate.md) | log_flume.c | 82.05% |
| 0x0040c250 | [FUN_0040c250](0x0040c250-FUN_0040c250.md) | log_flume.c | 42.86% |
| 0x0040c8d0 | [LogFlumeTrackRemoveObject](0x0040c8d0-LogFlumeTrackRemoveObject.md) | log_flume.c | 92.59% |
| 0x0040d6f0 | [FUN_0040d6f0](0x0040d6f0-FUN_0040d6f0.md) | log_flume.c | 97.67% |
| 0x0040e440 | [FUN_0040e440](0x0040e440-FUN_0040e440.md) | log_flume.c | 74.67% |
| 0x0040f050 | [FUN_0040f050](0x0040f050-FUN_0040f050.md) | log_flume.c | 83.96% |
| 0x0040f5b0 | [FUN_0040f5b0](0x0040f5b0-FUN_0040f5b0.md) | log_flume.c | 91.50% |
| 0x00411680 | [AdvanceFlumeMover](0x00411680-AdvanceFlumeMover.md) | log_flume.c | 63.04% |
| 0x00411fa0 | [FUN_00411fa0](0x00411fa0-FUN_00411fa0.md) | ride_queue.c | 87.74% |
| 0x00413450 | [FUN_00413450](0x00413450-FUN_00413450.md) | roads.c | 90.00% |
| 0x00413b50 | [DrivingSchoolRoadsCalcCursor](0x00413b50-DrivingSchoolRoadsCalcCursor.md) | roads.c | 94.67% |
| 0x00415220 | [SafariRideUpdate](0x00415220-SafariRideUpdate.md) | safari_ride.c | 75.56% |
| 0x00415ae0 | [RenderSpider](0x00415ae0-RenderSpider.md) | spider_ride.c | 98.62% |
| 0x00416fa0 | [RenderTempleSlide](0x00416fa0-RenderTempleSlide.md) | temple_slide.c | 95.00% |
| 0x00417430 | [TempleSlideUpdate](0x00417430-TempleSlideUpdate.md) | temple_slide.c | 79.39% |
| 0x00418710 | [IsBlokeNearElephantFountain](0x00418710-IsBlokeNearElephantFountain.md) | water_works.c | 82.69% |
| 0x004198a0 | [BoatingSchoolBuildStepPath](0x004198a0-BoatingSchoolBuildStepPath.md) | boating_school.c | 81.26% |
| 0x0041a720 | [BoatingSchoolUpdate](0x0041a720-BoatingSchoolUpdate.md) | boating_school.c | 96.58% |
| 0x0041df00 | [FUN_0041df00](0x0041df00-FUN_0041df00.md) | castle.c | 44.94% |
| 0x0041e4a0 | [FUN_0041e4a0](0x0041e4a0-FUN_0041e4a0.md) | castle.c | 80.00% |
| 0x0041ec50 | [CastleDispatchSetEditMode](0x0041ec50-CastleDispatchSetEditMode.md) | castle.c | 100% |
| 0x0041ec70 | [CastleDispatchCalcCursor](0x0041ec70-CastleDispatchCalcCursor.md) | castle.c | 100% |
| 0x0041eca0 | [FUN_0041eca0](0x0041eca0-FUN_0041eca0.md) | castle.c | 100% |
| 0x0041ece0 | [CastleDispatchAddObject](0x0041ece0-CastleDispatchAddObject.md) | castle.c | 100% |
| 0x0041ed00 | [CastleDispatchDCalcCursor](0x0041ed00-CastleDispatchDCalcCursor.md) | castle.c | 100% |
| 0x0041ed50 | [CastleDispatchRemoveObject](0x0041ed50-CastleDispatchRemoveObject.md) | castle.c | 100% |
| 0x0041ee40 | [FUN_0041ee40](0x0041ee40-FUN_0041ee40.md) | castle.c | 90.32% |
| 0x0041ef60 | [FUN_0041ef60](0x0041ef60-FUN_0041ef60.md) | castle.c | 43.10% |
| 0x00421e90 | [SubdivideCurveAtMaxError](0x00421e90-SubdivideCurveAtMaxError.md) | castle.c | 99.09% |
| 0x004227c0 | [EdgeMeshRemoveBackfaces](0x004227c0-EdgeMeshRemoveBackfaces.md) | castle.c | 95.06% |
| 0x00423a10 | [FUN_00423a10](0x00423a10-FUN_00423a10.md) | castle.c | 68.53% |
| 0x00424700 | [RenderCastleDummy](0x00424700-RenderCastleDummy.md) | castle.c | 88.41% |
| 0x00425e20 | [FUN_00425e20](0x00425e20-FUN_00425e20.md) | castle.c | 90.70% |
| 0x004269e0 | [lego_sqrtf_init](0x004269e0-lego_sqrtf_init.md) | castle.c | 100% |
| 0x004280b0 | [FUN_004280b0](0x004280b0-FUN_004280b0.md) | castle.c | 99.39% (effective) |
| 0x004284d0 | [FUN_004284d0](0x004284d0-FUN_004284d0.md) | castle.c | 71.38% |
| 0x00429cf0 | [FUN_00429cf0](0x00429cf0-FUN_00429cf0.md) | castle.c | 97.27% |
| 0x0042aa90 | [BalloonzUpdate](0x0042aa90-BalloonzUpdate.md) | balloonz.c | 94.65% |
| 0x0042bcf0 | [RenderCarousel](0x0042bcf0-RenderCarousel.md) | carousel.c | 42.80% |
| 0x0042c820 | [CarouselUpdate](0x0042c820-CarouselUpdate.md) | carousel.c | 85.71% |
| 0x0042f0f0 | [FUN_0042f0f0](0x0042f0f0-FUN_0042f0f0.md) | eatery.c | 88.29% |
| 0x0042fbb0 | [Restaurant2Update](0x0042fbb0-Restaurant2Update.md) | eatery.c | 41.89% |
| 0x004304e0 | [RenderEateryBaseLayers](0x004304e0-RenderEateryBaseLayers.md) | eatery.c | 87.10% |
| 0x00430b10 | [RenderRestaurant2](0x00430b10-RenderRestaurant2.md) | eatery.c | 97.36% |
| 0x004316f0 | [OctopusCafeUpdate](0x004316f0-OctopusCafeUpdate.md) | eatery.c | 60.85% |
| 0x00433840 | [JungleCruiseBuildStepPath](0x00433840-JungleCruiseBuildStepPath.md) | jungle_cruise.c | 83.61% |
| 0x00437260 | [SearchJunglePathConnected](0x00437260-SearchJunglePathConnected.md) | jungle_cruise.c | 38.41% |
| 0x00438f10 | [SaloonUpdate](0x00438f10-SaloonUpdate.md) | western_town.c | 73.79% |
| 0x00439ef0 | [LegoMediaShopUpdate](0x00439ef0-LegoMediaShopUpdate.md) | shops.c | 80.11% |
| 0x0043a7a0 | [FUN_0043a7a0](0x0043a7a0-FUN_0043a7a0.md) | space_tower.c | 37.65% |
| 0x0043aac0 | [FUN_0043aac0](0x0043aac0-FUN_0043aac0.md) | space_tower.c | 83.93% |
| 0x0043bac0 | [SpaceTowerUpdate](0x0043bac0-SpaceTowerUpdate.md) | space_tower.c | 75.32% |
| 0x0043be70 | [RenderSpinningBarrels](0x0043be70-RenderSpinningBarrels.md) | spinning_barrels.c | 99.20% |
| 0x0043c950 | [SpinningBarrelsUpdate](0x0043c950-SpinningBarrelsUpdate.md) | spinning_barrels.c | 84.84% |
| 0x0043da60 | [RenderPlaneRide](0x0043da60-RenderPlaneRide.md) | plane_ride.c | 96.54% |
| 0x0043e110 | [LoadZoomer](0x0043e110-LoadZoomer.md) | plane_ride.c | 97.89% |
| 0x0043e410 | [PlaneRideUpdate](0x0043e410-PlaneRideUpdate.md) | plane_ride.c | 79.57% |
| 0x0043ea30 | [ListBoxDialog](0x0043ea30-ListBoxDialog.md) | dialog.c | 69.84% |
| 0x004401b0 | [FUN_004401b0](0x004401b0-FUN_004401b0.md) | man3d.c | 90.20% |
| 0x00441d60 | [RenderUsingRin](0x00441d60-RenderUsingRin.md) | render3d.c | 100% |
| 0x00441f20 | [LoadPalette](0x00441f20-LoadPalette.md) | render3d.c | 81.00% |
| 0x00442040 | [RemapTexCoordsToCell](0x00442040-RemapTexCoordsToCell.md) | render3d.c | 45.93% |
| 0x00442980 | [FUN_00442980](0x00442980-FUN_00442980.md) | render3d.c | 60.78% |
| 0x00442cc0 | [GetScreenCoordsForObject](0x00442cc0-GetScreenCoordsForObject.md) | render3d.c | 79.01% |
| 0x004431f0 | [GetLegColourOfBloke](0x004431f0-GetLegColourOfBloke.md) | render3d.c | 27.78% |
| 0x00443220 | [GetArmColourOfBloke](0x00443220-GetArmColourOfBloke.md) | render3d.c | 30.30% |
| 0x004434d0 | [LoadBmpIntoImage](0x004434d0-LoadBmpIntoImage.md) | challenge.c | 84.04% |
| 0x00443bd0 | [OpenAviAnim](0x00443bd0-OpenAviAnim.md) | challenge.c | 91.98% |
| 0x00444a70 | [DrawAppraisalBar](0x00444a70-DrawAppraisalBar.md) | challenge.c | 80.21% |
| 0x004453a0 | [RunAppraisal](0x004453a0-RunAppraisal.md) | challenge.c | 15.70% |
| 0x0044e010 | [__BMPLoader](0x0044e010-__BMPLoader.md) | gfx.c | 39.57% |
| 0x0044e580 | [LoadColourTable](0x0044e580-LoadColourTable.md) | gfx.c | 83.78% |
| 0x0044fe80 | [FUN_0044fe80](0x0044fe80-FUN_0044fe80.md) | bloke_ai.c | 94.89% |
| 0x00451740 | [PrintCertificate](0x00451740-PrintCertificate.md) | certificate.c | 100% (asm) |
| 0x00451e70 | [SetupControllers](0x00451e70-SetupControllers.md) | controller.c | 91.89% |
| 0x00452030 | [FUN_00452030](0x00452030-FUN_00452030.md) | controller.c | 57.36% |
| 0x00453d10 | [WinMain](0x00453d10-WinMain.md) | main.c | 100% |
| 0x00453da0 | [stackdump](0x00453da0-stackdump.md) | exceptlog.c | 79.82% |
| 0x00454380 | [WriteModuleInfo](0x00454380-WriteModuleInfo.md) | exceptlog.c | 100% |
| 0x00455370 | [BubbleHelp](0x00455370-BubbleHelp.md) | text.c | 24.23% |
| 0x004557c0 | [HTBubbleHelp](0x004557c0-HTBubbleHelp.md) | text.c | 76.47% |
| 0x004567a0 | [RenderFullMap](0x004567a0-RenderFullMap.md) | mapscreen.c | 81.85% |
| 0x00457970 | [IsFootprintClear](0x00457970-IsFootprintClear.md) | bricks.c | 87.80% |
| 0x00457a70 | [FUN_00457a70](0x00457a70-FUN_00457a70.md) | bricks.c | 100% effective match (reccmp), bytes still differ |
| 0x00459970 | [FUN_00459970](0x00459970-FUN_00459970.md) | gamemap.c | 94.83% |
| 0x0045a4a0 | [CalculateMapRenderOrder](0x0045a4a0-CalculateMapRenderOrder.md) | gamemap.c | 62.28% |
| 0x0045acc0 | [GetTileBounds](0x0045acc0-GetTileBounds.md) | tilemap.c | 75.63% |
| 0x0045ad60 | [GetTileCentre](0x0045ad60-GetTileCentre.md) | tilemap.c | 93.02% |
| 0x0045ade0 | [FUN_0045ade0](0x0045ade0-FUN_0045ade0.md) | tilemap.c | 43.41% |
| 0x0045bcd0 | [PointToIsoPlane](0x0045bcd0-PointToIsoPlane.md) | tilemap.c | 46.84% |
| 0x0045c900 | [FUN_0045c900](0x0045c900-FUN_0045c900.md) | tilemap.c | 65.71% |
| 0x0045c9c0 | [FUN_0045c9c0](0x0045c9c0-FUN_0045c9c0.md) | tilemap.c | 49.38% |
| 0x0045ca90 | [FUN_0045ca90](0x0045ca90-FUN_0045ca90.md) | tilemap.c | 42.86% |
| 0x0045d5d0 | [FUN_0045d5d0](0x0045d5d0-FUN_0045d5d0.md) | tilemap.c | 42.28% |
| 0x0045d770 | [FUN_0045d770](0x0045d770-FUN_0045d770.md) | tilemap.c | 69.87% |
| 0x0045da60 | [RestoreBaseMap](0x0045da60-RestoreBaseMap.md) | tilemap.c | 95.65% |
| 0x0045dd80 | [AddObjectToMap](0x0045dd80-AddObjectToMap.md) | map_object.c | 99.10% |
| 0x0045eb30 | [BuildObject](0x0045eb30-BuildObject.md) | map_object.c | 96.81% |
| 0x0045ed30 | [ObjectIsBuilt](0x0045ed30-ObjectIsBuilt.md) | map_object.c | 93.42% |
| 0x0045f5f0 | [BuildCursorPtr](0x0045f5f0-BuildCursorPtr.md) | map_object.c | 87.65% |
| 0x0045f810 | [ValidateCursor](0x0045f810-ValidateCursor.md) | map_object.c | 94.15% |
| 0x0045fad0 | [FUN_0045fad0](0x0045fad0-FUN_0045fad0.md) | map_object.c | 93.59% |
| 0x0045fca0 | [FUN_0045fca0](0x0045fca0-FUN_0045fca0.md) | map_object.c | 94.76% |
| 0x0045ff00 | [RenderCursor](0x0045ff00-RenderCursor.md) | map_object.c | 95.48% |
| 0x004610f0 | [DrawPathTilesInRect](0x004610f0-DrawPathTilesInRect.md) | map_object.c | 89.95% |
| 0x00461290 | [ClampScrollPosition](0x00461290-ClampScrollPosition.md) | map_object.c | 43.92% |
| 0x00461a50 | [LoadBaseMap](0x00461a50-LoadBaseMap.md) | map_object.c | 84.15% |
| 0x00462c60 | [StartOverlayAnims](0x00462c60-StartOverlayAnims.md) | map_object.c | 95.35% |
| 0x00462ef0 | [DoMapAI](0x00462ef0-DoMapAI.md) | map_object.c | 93.43% |
| 0x00465850 | [FUN_00465850](0x00465850-FUN_00465850.md) | draw.c | 34.23% |
| 0x00465ee0 | [FUN_00465ee0](0x00465ee0-FUN_00465ee0.md) | draw.c | 84.56% |
| 0x00466560 | [PushSetTarget](0x00466560-PushSetTarget.md) | draw.c | 85.71% |
| 0x00466770 | [FUN_00466770](0x00466770-FUN_00466770.md) | draw.c | 75.06% |
| 0x00469c80 | [ScriptEventClear](0x00469c80-ScriptEventClear.md) | objectives.c | 94.32% |
| 0x0046a140 | [CallReportHandler](0x0046a140-CallReportHandler.md) | nerps.c | 100% |
| 0x0046a5b0 | [ScriptEventNeedIn](0x0046a5b0-ScriptEventNeedIn.md) | nerps.c | 74.32% |
| 0x0046b2d0 | [RunLevelScript](0x0046b2d0-RunLevelScript.md) | nerps.c | 100% |
| 0x0046c7e0 | [LoadObjectiveEventList](0x0046c7e0-LoadObjectiveEventList.md) | nerps.c | 98.29% |
| 0x0046c920 | [SaveScripts](0x0046c920-SaveScripts.md) | nerps.c | 89.23% |
| 0x0046d850 | [ScrollIconRegion](0x0046d850-ScrollIconRegion.md) | icon.c | 71.73% |
| 0x0046e0a0 | [RenderBuildObjectIcon](0x0046e0a0-RenderBuildObjectIcon.md) | icon.c | 83.66% |
| 0x0046f9a0 | [FUN_0046f9a0](0x0046f9a0-FUN_0046f9a0.md) | icon.c | 41.45% |
| 0x00471ca0 | [RemoveNewObject](0x00471ca0-RemoveNewObject.md) | popupinfo.c | 63.16% |
| 0x00471d90 | [FUN_00471d90](0x00471d90-FUN_00471d90.md) | popupinfo.c | 96.67% |
| 0x004724a0 | [DrawPopUpInfo](0x004724a0-DrawPopUpInfo.md) | popupinfo.c | 89.18% |
| 0x00473b00 | [UpdateControllerFromMouseData](0x00473b00-UpdateControllerFromMouseData.md) | input.c | 90.83% |
| 0x00476460 | [OpenAviMovie](0x00476460-OpenAviMovie.md) | interface.c | 89.77% |
| 0x00476d20 | [UpdateAviAudioBuffer](0x00476d20-UpdateAviAudioBuffer.md) | interface.c | 97.69% |
| 0x00477bd0 | [FindMapPathAStar](0x00477bd0-FindMapPathAStar.md) | gamemain.c | 88.02% |
| 0x0047cba0 | [LLIDB_LoadTSFData](0x0047cba0-LLIDB_LoadTSFData.md) | llidb.c | 93.40% |
| 0x0047d8e0 | [SaveGame](0x0047d8e0-SaveGame.md) | saveload.c | 91.55% |
| 0x0047e980 | [LoadGame](0x0047e980-LoadGame.md) | saveload.c | 81.34% |
| 0x0047f880 | [GameMain](0x0047f880-GameMain.md) | debug.c | 99.62% |
| 0x004809d0 | [LoadObjectClassAliasElements](0x004809d0-LoadObjectClassAliasElements.md) | objclass.c | 98.63% |
| 0x00484790 | [FUN_00484790](0x00484790-FUN_00484790.md) | bloke.c | 97.62% |
| 0x00484920 | [DoLowLevelAI](0x00484920-DoLowLevelAI.md) | bloke.c | 100% |
| 0x00484a70 | [SetBlokePositionFromBNV](0x00484a70-SetBlokePositionFromBNV.md) | bloke.c | 95.12% |
| 0x004856a0 | [PrintSpriteEx](0x004856a0-PrintSpriteEx.md) | print_sprite.c | 96.64% |
| 0x00488840 | [GenerateNewImageFromZBuffer](0x00488840-GenerateNewImageFromZBuffer.md) | render.c | 76.27% |
| 0x00488c80 | [RenderScaledSpriteOffset](0x00488c80-RenderScaledSpriteOffset.md) | render.c | 91.54% |
| 0x00489750 | [RES_OpenVolume](0x00489750-RES_OpenVolume.md) | resource.c | 96.75% |
| 0x0048ad00 | [InitFreePlayLists](0x0048ad00-InitFreePlayLists.md) | freeplay.c | 100% |
| 0x0048b2a0 | [FreePlayObjectList](0x0048b2a0-FreePlayObjectList.md) | freeplay.c | 82.76% |
| 0x0048cf10 | [PrintProfileDetails](0x0048cf10-PrintProfileDetails.md) | profile.c | 61.11% |
| 0x0048dd00 | [PrintSavedGameDetails](0x0048dd00-PrintSavedGameDetails.md) | savegame_ui.c | 92.33% |
| 0x0048e550 | [EnterSaveGameDetails](0x0048e550-EnterSaveGameDetails.md) | savegame_ui.c | 84.10% |
| 0x004921c0 | [ConvertWaveToPcm16](0x004921c0-ConvertWaveToPcm16.md) | sound_sfx.c | 92.26% |
| 0x00492380 | [CreateSampleFromWAV](0x00492380-CreateSampleFromWAV.md) | sound_sfx.c | 97.49% |
| 0x004966a0 | [UpdateSampleVolume](0x004966a0-UpdateSampleVolume.md) | sound_music.c | 64.15% |
| 0x00498420 | [SpeechParseWavHeader](0x00498420-SpeechParseWavHeader.md) | stream.c | 93.19% |
| 0x0049a7f0 | [Mechanic_Build](0x0049a7f0-Mechanic_Build.md) | worker.c | 96.15% |
| 0x0049b350 | [FUN_0049b350](0x0049b350-FUN_0049b350.md) | worker.c | 100% |
