1.compile-games.yml
This is the automatic compilation pipeline of GitHub Actions. When someone requests a PR to the main branch, it automatically finds the modified games in games/. Compile these games across platforms only (Win x86/Linux x86/Linux ARM), then backfill the executable files to compiled-games/< game name >/ in the repository, and finally attach the "compiled" tag to the PR.

2.validate-games.yml
Check and verify the config.txt file of the game in the PR submission. If it does not meet the requirements, the merge is not allowed.