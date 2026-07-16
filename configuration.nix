{ pkgs, modulesPath, ... }: {
  imports = [
    "${modulesPath}/installer/cd-dvd/installation-cd-graphical-gnome.nix"
  ];

  # 1. PHÒNG CHỐNG TRÊN 2 LỚP: VÁ TRIỆT ĐỂ LỖI ĐEN MÀN HÌNH MÁY DELL
  # Ép GNOME chạy trên X11 cực kỳ tương thích thay vì Wayland (Tránh kẹt dấu gạch '_')
  services.xserver.displayManager.gdm.wayland = false;
  # Tắt màn hình chờ Plymouth để rút ngắn thời gian boot và tránh treo driver đồ họa
  boot.plymouth.enable = false;

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
  # Hệ thống tự động ghim Firefox, VS Code, CP Editor và Thư mục lên thanh Dock cho bro!
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
