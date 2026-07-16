{ pkgs, modulesPath, lib, ... }: {
  imports = [
    "${modulesPath}/installer/cd-dvd/installation-cd-graphical-gnome.nix"
  ];

  # 1. TRÁNH TREO MÀN HÌNH KHI BOOT (Tắt Plymouth)
  boot.plymouth.enable = lib.mkForce false;

  # 2. ĐẢM BẢO WI-FI INTEL AX201 HOẠT ĐỘNG HOÀN HẢO
  nixpkgs.config.allowUnfree = true;
  hardware.enableAllFirmware = true;
  hardware.enableRedistributableFirmware = true;
  networking.networkmanager.enable = true;

  # 3. CÀI ĐẶT CÁC APP CHỦ LỰC
  environment.systemPackages = with pkgs; [
    gcc             # Trình biên dịch C++ (g++)
    python3         # Python 3
    vscode          # Trình soạn thảo VS Code
    firefox         # Trình duyệt Firefox (Sẽ hoạt động bình thường trở lại!)

    # SỬA LỖI CRASH CP EDITOR BẰNG CÁCH WRAP RIÊNG NÓ (KHÔNG ẢNH HƯỞNG ĐẾN FIREFOX)
    (symlinkJoin {
      name = "cpeditor";
      paths = [ cpeditor ];
      postBuild = ''
        # Xóa liên kết thực thi gốc trong package ảo này
        rm $out/bin/cpeditor
        # Tạo một wrapper mới gán kèm biến QT_QPA_PLATFORMTHEME trước khi chạy cpeditor thật
        ${makeWrapper}/bin/makeWrapper ${cpeditor}/bin/cpeditor $out/bin/cpeditor \
          --set QT_QPA_PLATFORMTHEME generic
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
