{ pkgs, modulesPath, lib, ... }: {
  imports = [
    "${modulesPath}/installer/cd-dvd/installation-cd-graphical-gnome.nix"
  ];

  # 1. TRÁNH TREO MÀN HÌNH KHI BOOT (Tắt Plymouth)
  # Dùng lib.mkForce để ép tắt Plymouth, giải quyết triệt để xung đột với cấu hình Live CD gốc!
  boot.plymouth.enable = lib.mkForce false;

  # 2. ĐẢM BẢO WI-FI INTEL AX201 HOẠT ĐỘNG HOÀN HẢO
  nixpkgs.config.allowUnfree = true;
  hardware.enableAllFirmware = true;
  hardware.enableRedistributableFirmware = true;
  networking.networkmanager.enable = true;

  # 3. CHỈ CÀI ĐẶT CÁC APP CHỦ LỰC (Giải quyết triệt để lỗi crash của CP Editor bằng Shell Wrapper)
  environment.systemPackages = with pkgs; [
    gcc             # Trình biên dịch C++ (g++)
    python3         # Python 3
    vscode          # Trình soạn thảo VS Code
    firefox         # Trình duyệt Firefox (Đã hoạt động bình thường!)

    # Đóng gói động CP Editor với biến môi trường cô lập
    (symlinkJoin {
      name = "cpeditor";
      paths = [ cpeditor ];
      postBuild = ''
        # Xóa liên kết thực thi gốc trong package ảo này
        rm $out/bin/cpeditor
        
        # Tạo file chạy dạng shell script để export biến môi trường riêng cho CP Editor
        echo '#!/bin/sh' > $out/bin/cpeditor
        echo 'export QT_QPA_PLATFORMTHEME=generic' >> $out/bin/cpeditor
        echo 'exec ${cpeditor}/bin/cpeditor "''$@"' >> $out/bin/cpeditor
        
        # Cấp quyền thực thi cho file wrapper
        chmod +x $out/bin/cpeditor
      '';
    })
  ];

  # 4. THIẾT LẬP HÌNH NỀN VÀ TỰ ĐỘNG GHIM APP LÊN THANH DOCK
  environment.etc."wallpaper.png".source = ./wallpaper.png;

  services.xserver.desktopManager.gnome.extraGSettingsOverrides = ''
    [org.gnome.desktop.background]
    picture-uri='file:///etc/wallpaper.png'
    picture-uri-dark='file:///etc/wallpaper.png'
    picture-options='zoom'

    [org.gnome.shell]
    favorite-apps=['firefox.desktop', 'code.desktop', 'cpeditor.desktop', 'org.gnome.Nautilus.desktop']
  '';

  # Tối ưu hóa tốc độ boot: Bỏ qua bước đợi kết nối mạng lúc khởi động
  systemd.services.NetworkManager-wait-online.enable = false;
}
