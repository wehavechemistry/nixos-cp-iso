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

  # 3. CHỈ CÀI ĐẶT CÁC APP CHỦ LỰC (Không cài app thừa tránh xung đột)
  environment.systemPackages = with pkgs; [
    gcc             # Trình biên dịch C++ (g++)
    python3         # Python 3
    vscode          # Trình soạn thảo VS Code
    cpeditor        # CP Editor chuyên dụng cho competitive programming
    firefox         # Trình duyệt Firefox (Đảm bảo chắc chắn có sẵn)
  ];

  # 4. THIẾT LẬP HÌNH NỀN VÀ TỰ ĐỘNG GHIM APP LÊN THANH DOCK (KHÔNG DÙNG CONSOLE)
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
