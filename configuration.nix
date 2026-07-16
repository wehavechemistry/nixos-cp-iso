{ pkgs, modulesPath, ... }: {
  imports = [
    "${modulesPath}/installer/cd-dvd/installation-cd-graphical-gnome.nix"
  ];

  # 1. Driver Wi-Fi unfree cho máy Dell (Đảm bảo tự nhận card mạng 100%)
  nixpkgs.config.allowUnfree = true;
  hardware.enableAllFirmware = true;
  hardware.enableRedistributableFirmware = true;
  networking.networkmanager.enable = true;

  # 2. Cài sẵn G++, Python, VS Code, CP Editor OFFLINE
  environment.systemPackages = with pkgs; [
    gcc             # Trình biên dịch C++ (G++)
    python3         # Ngôn ngữ Python 3
    vscode          # Trình soạn thảo VS Code (Cần allowUnfree ở trên)
    cpeditor        # CP Editor chuyên dụng (Đã sửa cú pháp cpeditor)
    git             # Git cơ bản
  ];

  # 3. Ép GNOME nhận hình nền custom từ file wallpaper.png trong Repository của bro
  environment.etc."wallpaper.png".source = ./wallpaper.png;

  services.xserver.desktopManager.gnome.extraGSettingsOverrides = ''
    [org.gnome.desktop.background]
    picture-uri='file:///etc/wallpaper.png'
    picture-uri-dark='file:///etc/wallpaper.png'
    picture-options='zoom'
  '';

  # Tối ưu hóa tốc độ boot: Không bắt hệ thống đợi kết nối mạng lúc khởi động
  systemd.services.NetworkManager-wait-online.enable = false;
}
