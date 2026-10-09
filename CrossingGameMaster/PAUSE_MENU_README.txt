CROSSING GAME - CAP NHAT PAUSE MENU VA LEVEL TRANSITION
=======================================================

1. SUA LOI QUA LEVEL
--------------------
- Khi tang level, xe va dong vat KHONG bi reset ve startPositions nua.
- Toc do van tang theo level, nhung vi tri dang chay duoc giu nguyen.
- Nhu vay nguoi choi khong con khoang trong do tat ca chuong ngai vat bien mat.

2. KHOA DI CHUYEN KHI CHUONG NGAI VAT CHUA SAN SANG
--------------------------------------------------
- Khi bat dau New Game hoac sau khi Load Game, nguoi choi bi khoa di chuyen.
- Game chi mo khoa khi MOI lane da co it nhat mot chuong ngai vat hien trong man hinh.
- Khi qua level, game khoa toi thieu 0.45 giay va kiem tra lai trang thai chuong ngai vat.
- Trong thoi gian cho, dong chu "GET READY... WAIT FOR OBSTACLES" duoc hien thi.

3. THIET KE PAUSE / SETTINGS MOI
-------------------------------
- Nut Pause goc phai duoc ve bang SFML theo phong cach may - day leo - la cay.
- Pause Menu co nen may xanh, banner may hong, vien day leo va icon SFML.
- Settings co nut tang/giam am luong, Mute/Unmute va Back to Pause.
- UI duoc ve truc tiep bang SFML Shapes nen khong bi hien nen caro cua anh tham chieu.
- Ba anh nguoi dung gui duoc luu trong thu muc PauseDesignReferences de doi chieu.

4. DIEU KHIEN
-------------
- Esc hoac nut Pause: mo/tat Pause Menu.
- Up/Down + Enter: chon muc Pause Menu.
- Left/Right: giam/tang am luong trong Settings.
- M: Mute/Unmute.
- Esc/Backspace trong Settings: quay lai Pause Menu.

5. BUILD
--------
- Mo file CrossingGameMaster.slnx.
- Chon x64 Debug hoac x64 Release.
- Build > Rebuild Solution.
- Project dang cau hinh SFML tai C:\SFML-3.1.0.
