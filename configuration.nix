{ pkgs, modulesPath, ... }: {
  imports = [
    "${modulesPath}/installer/cd-dvd/installation-cd-graphical-gnome.nix"
  ];

  # 1. Driver Wi-Fi cho máy Dell
  nixpkgs.config.allowUnfree = true;
  hardware.enableRedistributableFirmware = true;
  networking.networkmanager.enable = true;

  # 2. Cài sẵn G++, Python, VS Code, CP Editor OFFLINE
  environment.systemPackages = with pkgs; [
    gcc             # Biên dịch C++
    python3         # Chạy Python
    vscode          # Trình soạn thảo VS Code
    cp-editor       # Công cụ luyện CP siêu nhẹ
    git             # Git cơ bản
  ];

  systemd.services.NetworkManager-wait-online.enable = false;
}
