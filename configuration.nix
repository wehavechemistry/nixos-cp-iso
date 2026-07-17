{ pkgs, modulesPath, lib, ... }: {
  imports = [
    # DÙNG BẢN BASE CHUẨN (Đảm bảo GitHub build mượt mà không bao giờ lỗi path)
    "${modulesPath}/installer/cd-dvd/installation-cd-base.nix"
  ];

  # 1. BẬT GIAO DIỆN XFCE THỦ CÔNG & ĐỔI USER THÀNH 'dtth' CHO GIỐNG HỆT THẦY
  services.xserver.enable = true;
  services.xserver.desktopManager.xfce.enable = true;
  services.xserver.displayManager.lightdm.enable = true;
  
  # Tạo hẳn user 'dtth' để khi vào terminal hay màn hình khóa đều khớp với ảnh của thầy
  users.users.dtth = {
    isNormalUser = true;
    extraGroups = [ "wheel" "networkmanager" "video" ];
    description = "dtth";
    initialPassword = ""; # Không để mật khẩu để click đăng nhập cho nhanh
  };

  # Ép hệ thống tự động đăng nhập thẳng vào user dtth khi vừa boot xong
  services.displayManager.autoLogin.enable = true;
  services.displayManager.autoLogin.user = lib.mkForce "dtth";

  # 2. TRÁNH TREO MÀN HÌNH KHI BOOT (Tắt Plymouth)
  boot.plymouth.enable = lib.mkForce false;

  # 3. ĐẢM BẢO WI-FI INTEL AX201 HOẠT ĐỘNG HOÀN HẢO
  nixpkgs.config.allowUnfree = true;
  hardware.enableAllFirmware = true;
  hardware.enableRedistributableFirmware = true;
  networking.networkmanager.enable = true;

  # 4. CÀI ĐẶT CÁC APP CHỦ LỰC THEO BỘ CÔNG CỤ ĐI THI CỦA THẦY
  environment.systemPackages = with pkgs; [
    gcc             # Trình biên dịch C++
    python3         # Python 3
    firefox         # Trình duyệt Firefox
    vscode          # Trình soạn thảo VS Code
    cpeditor        # CP Editor
    codeblocks      # Code::Blocks
  ];

  # 5. TỰ ĐỘNG ÉP NỀN ĐEN KỊT KHI KHỞI ĐỘNG
  environment.etc."set-black-bg.sh" = {
    text = ''
      #!/bin/sh
      # Tự tạo file ảnh 1x1 pixel màu đen nguyên bản từ mã Hex
      printf '\x89\x50\x4e\x47\x0d\x0a\x1a\x0a\x00\x00\x00\x0d\x49\x48\x44\x52\x00\x00\x00\x01\x00\x00\x00\x01\x08\x02\x00\x00\x00\x90\x77\x53\xde\x00\x00\x00\x0c\x49\x44\x41\x54\x78\x9c\x63\x60\x60\x60\x00\x00\x00\x04\x00\x01\x27\x34\x27\x0a\x00\x00\x00\x00\x49\x45\x4e\x44\xae\x42\x60\x82' > /tmp/black.png
      
      # Đợi giao diện đồ họa XFCE load xong hoàn toàn
      sleep 2
      
      # Quét và ép tất cả các không gian màn hình nhận ảnh đen, bật chế độ Zoom
      for p in $(xfconf-query -c xfce4-desktop -p /backdrop -l 2>/dev/null | grep last-image); do
        xfconf-query -c xfce4-desktop -p "$p" -s /tmp/black.png
      done
      for p in $(xfconf-query -c xfce4-desktop -p /backdrop -l 2>/dev/null | grep image-style); do
        xfconf-query -c xfce4-desktop -p "$p" -s 5
      done
    '';
    mode = "0755";
  };

  # Lệnh gọi script chạy ngầm ngay khi vừa đăng nhập vào màn hình desktop
  environment.etc."xdg/autostart/black-background.desktop".text = ''
    [Desktop Entry]
    Type=Application
    Name=Set Black Background
    Exec=/etc/set-black-bg.sh
    OnlyShowIn=XFCE;
    StartupNotify=false
    Terminal=false
  '';

  # Tối ưu hóa tốc độ boot: Bỏ qua bước đợi kết nối mạng lúc khởi động
  systemd.services.NetworkManager-wait-online.enable = false;
}
