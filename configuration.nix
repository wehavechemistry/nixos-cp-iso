{ pkgs, modulesPath, lib, ... }: {
  imports = [
    # Boot thẳng vào môi trường XFCE giống hệt thầy
    "${modulesPath}/installer/cd-dvd/installation-cd-graphical-xfce.nix"
  ];

  # Khởi động siêu tốc, bỏ qua màn hình chờ Plymouth
  boot.plymouth.enable = lib.mkForce false;

  # Mở khóa Unfree Driver để nhận card mạng Intel AX201 trên laptop Dell
  nixpkgs.config.allowUnfree = true;
  hardware.enableAllFirmware = true;
  hardware.enableRedistributableFirmware = true;
  networking.networkmanager.enable = true;

  # Bộ công cụ đi thi chuẩn đích danh của thầy
  environment.systemPackages = with pkgs; [
    gcc
    python3
    firefox
    vscode
    cpeditor
    codeblocks
  ];

  # TỰ ĐỘNG ÉP NỀN ĐEN KỊT KHI KHỞI ĐỘNG (Bản vá nâng cao cho XFCE)
  environment.etc."set-black-bg.sh" = {
    text = ''
      #!/bin/sh
      # Tự tạo file ảnh 1x1 pixel màu đen nguyên bản từ mã Hex
      printf '\x89\x50\x4e\x47\x0d\x0a\x1a\x0a\x00\x00\x00\x0d\x49\x48\x44\x52\x00\x00\x00\x01\x00\x00\x00\x01\x08\x02\x00\x00\x00\x90\x77\x53\xde\x00\x00\x00\x0c\x49\x44\x41\x54\x78\x9c\x63\x60\x60\x60\x00\x00\x00\x04\x00\x01\x27\x34\x27\x0a\x00\x00\x00\x00\x49\x45\x4e\x44\xae\x42\x60\x82' > /tmp/black.png
      
      # Đợi giao diện đồ họa XFCE load xong hoàn toàn
      sleep 2
      
      # Quét sạch và ép tất cả các không gian màn hình nhận ảnh đen, bật chế độ Zoom
      for p in $(xfconf-query -c xfce4-desktop -p /backdrop -l 2>/dev/null | grep last-image); do
        xfconf-query -c xfce4-desktop -p "$p" -s /tmp/black.png
      done
      for p in $(xfconf-query -c xfce4-desktop -p /backdrop -l 2>/dev/null | grep image-style); do
        xfconf-query -c xfce4-desktop -p "$p" -s 5
      done
    '';
    mode = "0755"; # Cấp quyền thực thi cho script
  };

  # Lệnh gọi script chạy ngầm ngay khi đăng nhập vào màn hình desktop
  environment.etc."xdg/autostart/black-background.desktop".text = ''
    [Desktop Entry]
    Type=Application
    Name=Set Black Background
    Exec=/etc/set-black-bg.sh
    OnlyShowIn=XFCE;
    StartupNotify=false
    Terminal=false
  '';

  # Bỏ qua bước chờ kết nối mạng để tối ưu hóa thời gian mở máy
  systemd.services.NetworkManager-wait-online.enable = false;
}
