# SnakeUI: where it connects to SnakeLogic

Every connection point is marked in the code with a `// LOGIC:` comment. Find them all with:

```
rg "LOGIC:" SnakeUI
```

## Structure

| Folder | Content |
|---|---|
| `app/` | `MainWindow` (wires the screens), `ScreenNavigator`, `Catalog` (colors, achievements), `ProfileStore` (interim JSON profile), `Theme` |
| `screens/` | `MainMenuScreen`, `SettingsScreen`, `ShopScreen`, `AchievementsScreen`, `PlaceholderScreen` (game, not built yet). `PanelScreen` and `ListScreen` are the shared bases |
| `widgets/` | Reusable pieces: buttons (`SvgButton` base), `OptionRow`, `ListRow`, `ArtImage` / `ArtWidget`, backgrounds |
| `resources/` | SVG icons and pixel art, referenced from `SnakeUI.qrc` |

## Connection points

| Marker | File | What the logic provides |
|---|---|---|
| Profile storage | `main.cpp`, `app/ProfileStore.h` | `createServices(profilePath)` replaces `ProfileStore`. The file format already follows the profile from the project structure |
| Play | `MainWindow::connectMenu` | `createGame(settings, services)`, then `QTimer` -> `IGame::tick()` and keys -> `IGame::changeDirection()` |
| Settings | `MainWindow::connectSettings` | The selections become the `GameSettings` (mode, board, appleCount, speed) |
| Scores and balance | `MainWindow::showScores`, `ProfileData::bestScore` | `IProgressService` (balance, high score per mode and board) |
| Shop | `MainWindow::connectShop` | `ISkinService` replaces `ProfileStore::purchaseColor` |
| Achievement unlocked | `MainWindow::onAchievementUnlocked` | `IAchievementObserver::onAchievementUnlocked` (through `QtGameObserver`) |
| Achievement and color data | `app/Catalog.h` | Conditions, rewards, skins and prices move to `IAchievementService` / `ISkinService`. The UI keeps only names and art |
| Audio | `MainWindow::setAudioEnabled` | No audio engine yet |
| Game screen | `screens/PlaceholderScreen.h` | A `GameScreen` with a `BoardWidget` (QPainter) and the `QtGameObserver` adapter |

## Rules kept

- Screens never write the profile. `MainWindow` is the only place that changes it and saves.
- All text shown to the player is English.
- A new color or achievement is one line in `app/Catalog.cpp`. `Catalog::isConsistent()` is asserted at startup in Debug.
