## TODO
### Emergencies
- [x] Jump from any page to About-Page, will cause high cpu usage(100%)
- [x] If you go to New-Page, and come back to Download-Page, the cursor will blinking.
- [x] Just for first time, Download-Page shows all items.
- [x] After adding new item in New-Page, if you come back to Download-Page, it won't load latest data.
- [x] After adding new item in New-Page or discarding it, if you come back to download page, the whole page will be empty. (Just for the first time.)
- [x] Don't let users to enter empty links.
- [x] Show the percentage of files that are downloading in Download-Page.(live data)
- [x] Store live data of downloading files in a buffer and when program exit, save them to database.(To reduce the overhead of database read/write)
- [x] Justify text in middle of cells.
- [x] If you download a file and want to start another download, the application will crash.
- [x] If you downloading a file, you can't up/down in list.
- [x] If you download a file and it completed, you shouldn't download it again.
- [x] Some files has special charachter like %20 and won't show in list.
- [x] Set default download directory to: ~/Downloads
- [x] Currently if you download multiple files, only last file will get updated status.
- [x] Ability to pause/resume downloads.
- [x] If you resume a file, it shouldn't start from zero, it should update current downloaded value.
- [ ] In Download-Page, If you go to end of the page, it won't load data properly.
- [ ] If you start a download file and pause it and start it again, completed length won't be update anymore.
- [ ] Sometimes thread die without any reason.(Mostly in this case: Start/pause/Start...)
- [ ] If in the middle of downloading files go to another page, it will updating the whole screen. and after come back numbers reset to zero.

### Test
- [x] Normal download works
- [x] Pause/exit/resume works.
- [ ] Pause/resume works.
- [ ] Pause/Go to another page/back/resuem works.
- [ ] Start/Go to another page/back works.

### Enhancements
- [ ] Colorize items in list.(Deleted items: RED, completed items: blue,...)
- [ ] Notify: when usres enter empty link in New-Page.
- [?] In New-Page with pressing enter, the input should be accepted.
- [ ] Replace all pop-up windows with seperate windows(Due to constantly update of main window, it'll cluttering whole screen)

### New Features
- [ ] Create settings page that contain these things: 1. Proxy section 2. Defualt download directory.
- [ ] Remove items from download list.(Users can decide whether only item will delete or item + fill will delete together)
- [ ] Allow users to pass options to download items.
- [ ] New-Page: Specify destination folder.
- [ ] New-Page: Add Proxy section.
- [ ] New-Page: Specify Max Download/Upload speed.
- [ ] New-Page: Specify Max Connectinos.
