{ pkgs, modulesPath, lib, ... }: {
  imports = [
    "${modulesPath}/installer/cd-dvd/installation-cd-base.nix"
  ];

  # FIX GIỜ GIẤC PHÒNG THI: Khóa chuẩn múi giờ Việt Nam và đọc giờ Local từ BIOS Windows
  time.timeZone = "Asia/Ho_Chi_Minh";
  time.hardwareClockInLocalTime = true;

  # 1. KÍCH HOẠT GIAO DIỆN XFCE VÀ THIẾT LẬP USER 'dtth' CÓ MẬT KHẨU "1"
  services.xserver.enable = true;
  services.xserver.desktopManager.xfce.enable = true;
  services.xserver.displayManager.lightdm.enable = true;
  
  users.users.dtth = {
    isNormalUser = true;
    extraGroups = [ "wheel" "networkmanager" "video" ];
    description = "dtth";
    initialPassword = "1"; # Mật khẩu đăng nhập hệ điều hành là 1
  };

  # TẮT AUTO-LOGIN: Dừng ở màn hình đăng nhập để đảm bảo an toàn phần cứng khi boot
  services.displayManager.autoLogin.enable = false;
  boot.plymouth.enable = lib.mkForce false;

  # 2. ĐẢM BẢO PHẦN CỨNG LAPTOP VÀ WI-FI HOẠT ĐỘNG HOÀN HẢO
  nixpkgs.config.allowUnfree = true;
  hardware.enableAllFirmware = true;
  hardware.enableRedistributableFirmware = true;
  networking.networkmanager.enable = true;

  # 3. BỘ ỨNG DỤNG LẬP TRÌNH THI ĐẤU
  environment.systemPackages = with pkgs; [
    gcc
    python3
    firefox
    vscode
    cpeditor
    codeblocks
  ];

  # ==================== SYSTEMD SERVICE: ÉP ĐỘ SÁNG PHẦN CỨNG VỀ ĐÚNG MỨC 10 ====================
  systemd.services.set-safe-brightness = {
    description = "Ep cuong che do sang man hinh ve muc 10 siêu tiet kiem pin";
    after = [ "display-manager.service" ];
    wantedBy = [ "graphical.target" ];
    serviceConfig = {
      Type = "oneshot";
      ExecStart = "${pkgs.bash}/bin/bash -c 'sleep 2; echo 10 > /sys/class/backlight/intel_backlight/brightness || true'";
    };
  };

  # ==================== PHÉP THUẬT AUTO-LOAD CONFIG VÀ DI CHUYỂN REPO ====================
  system.activationScripts.setupDtthHome = {
    text = ''
      mkdir -p /home/dtth/.config/cpeditor
      mkdir -p /home/dtth/.config/xfce4/xfconf/xfce-perchannel-xml
      mkdir -p /home/dtth/CP_Folder

      # A. BÊ NGUYÊN THƯ MỤC CP_FOLDER TỪ REPO GITHUB THẢ VÀO HOME TRONG LIVE ISO
      cp -r ${./CP_Folder}/* /home/dtth/CP_Folder/

      # B. GHI ĐÈ FILE CẤU HÌNH CP EDITOR (SỬA NGOẶC, ÉP FONT 13 VÀ THIẾT LẬP BREEZE DARK)
      cat << 'EOF' > /home/dtth/.config/cpeditor/settings.ini
      [General]
      auto_complete_parentheses=true
      auto_remove_parentheses=true
      tab_jump_out_parentheses=true
      
      # Thiết lập Theme Breeze Dark và UI Dark Fusion chuẩn đét theo ảnh yêu cầu
      editor_theme=Breeze Dark
      ui_style=Dark Fusion
      
      # Ép cứng toàn bộ kích thước font chữ lên size 13 bằng chuỗi Qt thuần
      editor_font="DejaVu Sans Mono,13,-1,5,50,0,0,0,0,0"
      test_cases_font="DejaVu Sans Mono,13,-1,5,50,0,0,0,0,0"
      message_logger_font="DejaVu Sans Mono,13,-1,5,50,0,0,0,0,0"
      show_only_monospaced_font=true
      use_custom_application_font=false
      
      # Các cài đặt hệ thống bổ sung nhằm tối ưu hiệu năng ứng dụng
      answer_file_save_path=./''${basename}_''${1-index}.ans
      ask_for_loading_external_changes=true
      auto_load_external_changes_if_no_unsaved_modification=true
      auto_save=false
      auto_save_interval=1000
      auto_save_interval_type=After the last modification
      auto_uncheck_accepted_testcases=false
      beta=false
      check_on_testcases_with_empty_output=false
      check_update=true
      cursor_width=1
      default_file_paths_for_problem_urls=@Invalid()
      default_language=C++
      default_time_limit=5000
      detached_run_terminal_arguments=-e
      detached_run_terminal_program=xterm
      display_eoln_in_diff=false
      display_stopwatch=false
      display_test_case_length_limit=500000
      error_message_color=red
      extra_bottom_margin=false
      file_problem_binding=@Invalid()
      first_time_user=false
      force_close=false
      format_on_auto_save=false
      format_on_manual_save=false
      full_screen_dialog_shown=false
      geometry=@Rect(0 0 0 0)
      hide_stopwatch_result=false
      highlight_error_line=false
      html_diff_viewer_length_limit=50000
      input_file_save_path=./''${basename}_''${1-index}.in
      locale=system
      maximized_window=false
      message_length_limit=20000
      number_of_problems_in_contest=5
      opacity=100
      open_file_length_limit=20000
      open_old_file_for_old_problem_url=false
      output_display_length_limit=50000
      output_length_limit=500000
      promotion_dialog_shown=false
      replace_tabs=false
      restore_old_problem_url=false
      right_splitter_size=@ByteArray()
      run_on_empty_testcase=false
      save_faster=false
      save_file_on_compilation=true
      save_file_on_execution=false
      save_tests=false
      toggle_stopwatch_on_tab_switch=false
      total_usage_time=0
      view_mode=split
      warn_message_color=green
      wrap_text=false

      [cf]
      path=cf
      show_toast_messages=true

      [clang_format]
      arguments=
      program=clang-format
      style=BasedOnStyle: Google

      [competitive_companion]
      connection_port=10045
      enable=true
      head_comments=Problem: ''${json.name}\nContest: ''${json.group}\nURL: ''${json.url}\nMemory Limit: ''${json.memoryLimit} MB\nTime Limit: ''${json.timeLimit} ms
      head_comments_powered_by_cp_editor=true
      head_comments_time_format=yyyy-MM-dd HH:mm:ss
      open_new_tab=true
      set_time_limit_for_tab=false

      [cpp]
      compile_command=c++ -O3 -Wall
      compiler_output_codec=UTF-8
      output_path=''${tmpdir}/''${basename}
      run_arguments=
      template_cursor_position_offset_characters=4
      template_cursor_position_offset_type=end
      template_cursor_position_regex=main\\(\\)
      template_path=

      [hot_exit]
      auto_save=false
      auto_save_interval=20000
      enable=true
      EOF

      # C. ĐỒNG BỘ WINDOW MANAGER ADWAITA-DARK TẠO NỀN GIAO DIỆN TỐI ĐỒNG BỘ VỚI CP EDITOR
      cat << 'EOF' > /home/dtth/.config/xfce4/xfconf/xfce-perchannel-xml/xsettings.xml
      <?xml version="1.0" encoding="UTF-8"?>
      <channel name="xsettings" version="1.0">
        <property name="Net" type="empty">
          <property name="ThemeName" type="string" value="Adwaita-dark"/>
          <property name="IconThemeName" type="string" value="Adwaita"/>
        </property>
      </channel>
      EOF

      # D. ÉP HÌNH NỀN HỆ THỐNG LOAD THÀNH FILE WALLPAPER TRÊN REPO
      cp ${./wallpaper.png} /home/dtth/.config/wallpaper.png
      cat << 'EOF' > /home/dtth/.config/xfce4/xfconf/xfce-perchannel-xml/xfce4-desktop.xml
      <?xml version="1.0" encoding="UTF-8"?>
      <channel name="xfce4-desktop" version="1.0">
        <property name="backdrop" type="empty">
          <property name="screen0" type="empty">
            <property name="monitoreDP-1" type="empty">
              <property name="workspace0" type="empty">
                <property name="image-style" type="int" value="5"/>
                <property name="last-image" type="string" value="/home/dtth/.config/wallpaper.png"/>
                <property name="color-style" type="int" value="0"/>
                <property name="rgba1" type="array">
                  <value type="double" value="0.000000"/>
                  <value type="double" value="0.000000"/>
                  <value type="double" value="0.000000"/>
                  <value type="double" value="1.000000"/>
                </property>
              </property>
            </property>
            <property name="monitor0" type="empty">
              <property name="workspace0" type="empty">
                <property name="image-style" type="int" value="5"/>
                <property name="last-image" type="string" value="/home/dtth/.config/wallpaper.png"/>
                <property name="color-style" type="int" value="0"/>
                <property name="rgba1" type="array">
                  <value type="double" value="0.000000"/>
                  <value type="double" value="0.000000"/>
                  <value type="double" value="0.000000"/>
                  <value type="double" value="1.000000"/>
                </property>
              </property>
            </property>
          </property>
        </property>
      </channel>
      EOF

      # E. PHÂN QUYỀN SỞ HỮU TOÀN BỘ FILE CHO USER TRÁNH LỖI READ-ONLY
      chown -R dtth:users /home/dtth
    '';
    deps = [ "users" ];
  };

  # Vô hiệu hóa việc chờ kết nối mạng khi khởi động để giảm thời gian nạp ISO xuống mức tối thiểu
  systemd.services.NetworkManager-wait-online.enable = false;
}
