{ pkgs, modulesPath, lib, ... }: {
  imports = [
    "${modulesPath}/installer/cd-dvd/installation-cd-base.nix"
  ];

  # 1. KÍCH HOẠT GIAO DIỆN XFCE VÀ THIẾT LẬP USER 'dtth' CÓ MẬT KHẨU "1"
  services.xserver.enable = true;
  services.xserver.desktopManager.xfce.enable = true;
  services.xserver.displayManager.lightdm.enable = true;
  
  users.users.dtth = {
    isNormalUser = true;
    extraGroups = [ "wheel" "networkmanager" "video" ];
    description = "dtth";
    initialPassword = "1"; # Đặt mật khẩu là số 1 chuẩn bài như bạn muốn
  };

  # TẮT AUTO-LOGIN: Để hệ thống dừng ở màn hình khóa bắt nhập số 1 mới cho vào
  services.displayManager.autoLogin.enable = false;
  boot.plymouth.enable = lib.mkForce false;

  # 2. ĐẢM BẢO PHẦN CỨNG LAPTOP VÀ WI-FI HOẠT ĐỘNG HOÀN HẢO
  nixpkgs.config.allowUnfree = true;
  hardware.enableAllFirmware = true;
  hardware.enableRedistributableFirmware = true;
  networking.networkmanager.enable = true;

  # 3. BỘ ỨNG DỤNG ĐI THI KHÔNG LỖI CRASH
  environment.systemPackages = with pkgs; [
    gcc
    python3
    firefox
    vscode
    cpeditor
    codeblocks
  ];

  # ==================== SYSTEMD SERVICE: ÉP ĐỘ SÁNG PHẦN CỨNG VỀ ĐÚNG MỨC 20 ====================
  # Chạy độc lập sau khi Display Manager lên để không lo bị driver đè lại giá trị
  systemd.services.set-safe-brightness = {
    description = "Ep cuong che do sang man hinh ve muc 20 sau khi load driver";
    after = [ "display-manager.service" ];
    wantedBy = [ "graphical.target" ];
    serviceConfig = {
      Type = "oneshot";
      ExecStart = "${pkgs.bash}/bin/bash -c 'sleep 2; echo 20 > /sys/class/backlight/intel_backlight/brightness || true'";
    };
  };

  # ==================== PHÉP THUẬT KÍCH HOẠT DARK MODE TRỰC TIẾP VÀO HOME ====================
  system.activationScripts.setupDtthHome = {
    text = ''
      # Tạo sẵn các thư mục cấu hình trong thư mục cá nhân của dtth trước khi desktop load up
      mkdir -p /home/dtth/.config/cpeditor
      mkdir -p /home/dtth/.config/xfce4/xfconf/xfce-perchannel-xml

      # A. GHI ĐÈ FILE CẤU HÌNH CP EDITOR DRACULA CHUẨN CỦA BẠN (Đã xử lý chuỗi ký tự Nix)
      cat << 'EOF' > /home/dtth/.config/cpeditor/settings.ini
      [General]
      answer_file_save_path=./''${basename}_''${1-index}.ans
      ask_for_loading_external_changes=true
      auto_complete_parentheses=true
      auto_indent=true
      auto_load_external_changes_if_no_unsaved_modification=true
      auto_remove_parentheses=true
      auto_save=false
      auto_save_interval=1000
      auto_save_interval_type=After the last modification
      auto_uncheck_accepted_testcases=false
      beta=false
      check_on_testcases_with_empty_output=false
      check_update=true
      cursor_width=1
      custom_application_font=@Variant(\0\0\0@\0\0\0\b\0S\0\x61\0n\0s@$\0\0\0\0\0\0\xff\xff\xff\xff\x5\x1\0\x32\x10)
      default_file_paths_for_problem_urls=@Invalid()
      default_language=C++
      default_time_limit=5000
      detached_run_terminal_arguments=-e
      detached_run_terminal_program=xterm
      display_eoln_in_diff=false
      display_stopwatch=false
      display_test_case_length_limit=500000
      editor_font=@Variant(\0\0\0@\0\0\0\x12\0m\0o\0n\0o\0s\0p\0\x61\0\x63\0\x65@$\0\0\0\0\0\0\xff\xff\xff\xff\x2\x1\0\x32\x10)
      editor_theme=Dracula
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
      message_logger_font=@Variant(\0\0\0@\0\0\0\x12\0m\0o\0n\0o\0s\0p\0\x61\0\x63\0\x65@$\0\0\0\0\0\0\xff\xff\xff\xff\x2\x1\0\x32\x10)
      number_of_problems_in_contest=5
      opacity=100
      open_file_length_limit=20000
      open_old_file_for_old_problem_url=false
      output_display_length_limit=50000
      output_length_limit=500000
      promotion_dialog_shown=false
      recent_files=/home/dtth/A.cpp
      replace_tabs=false
      restore_old_problem_url=false
      right_splitter_size=@ByteArray()
      run_on_empty_testcase=false
      save_faster=false
      save_file_on_compilation=true
      save_file_on_execution=false
      save_tests=false
      show_compile_and_run_only=false
      show_only_monospaced_font=true
      splitter_size=@ByteArray()
      tab_jump_out_parentheses=false
      tab_width=4
      test_case_maximum_height=300
      test_cases_font=@Variant(\0\0\0@\0\0\0\x12\0m\0o\0n\0o\0s\0p\0\x61\0\x63\0\x65@$\0\0\0\0\0\0\xff\xff\xff\xff\x2\x1\0\x32\x10)
      testcases_matching_rules=@Variant(\0\0\0\v\0\0\0\x2\0\0\0\x10\0(\0.\0*\0)\0\\\0.\0i\0n\0\0\0\f\0\\\0\x31\0.\0\x61\0n\0s), @Variant(\0\0\0\v\0\0\0\x2\0\0\0\x10\0(\0.\0*\0)\0\\\0.\0i\0n\0\0\0\f\0\\\0\x31\0.\0o\0u\0t)
      toggle_stopwatch_on_tab_switch=false
      total_usage_time=0
      ui_style=Dark Fusion
      use_custom_application_font=false
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
      compile_command=c++ -Wall
      compiler_output_codec=UTF-8
      output_path=''${tmpdir}/''${basename}
      parentheses=@Variant(\0\0\0\t\0\0\0\x5\0\0\0\x2\0\0\0(\0\0\0\x2\0\0\0)\0\0\0\x2\0\0\0\x1\0\0\0\x2\0\0\0\x1\0\0\0\x2\0\0\0\x1), @Variant(\0\0\0\t\0\0\0\x5\0\0\0\x2\0\0\0Camp\\(\\)
      run_arguments=
      template_cursor_position_offset_characters=4
      template_cursor_position_offset_type=end
      template_cursor_position_regex=main\\(\\)
      template_path=

      [default_path]
      action\add_pairs_of_test_cases\changes=testcase
      action\add_pairs_of_test_cases\uses=''${testcase}
      action\custom_checker\changes=checker
      action\custom_checker\uses=''${checker}
      action\export_and_import_settings\changes=settings
      action\export_and_import_settings\uses=''${settings}
      action\export_and_load_session\changes=session
      action\export_and_load_session\uses=''${session}
      action\extract_and_load_snippets\changes=snippets
      action\extract_and_load_snippets\uses=''${snippets}
      action\load_single_test_case\changes=testcase
      action\load_single_test_case\uses=''${testcase}
      action\open_contest\changes="contest, file, testcase, checker"
      action\open_contest\uses=''${contest}
      action\open_file\changes="file, testcase, checker"
      action\open_file\uses=''${file}
      action\save_file\changes="file, testcase, checker"
      action\save_file\uses=''${file}
      action\save_test_case_to_a_file\changes=testcase
      action\save_test_case_to_a_file\uses=''${testcase}
      names_and_paths=@Variant(\0\0\0\v\0\0\0\x2\0\0\0\xe\0\x63\0h\0\x65\0\x63\0k\0\x65\0r\0\0\0\x14\0/\0h\0o\0m\0\x65\0/\0\x64\0t\0t\0h), @Variant(\0\0\0\v\0\0\0\x2\0\0\0\b\0\x66\0i\0l\0\x65\0\0\0\x14\0/\0h\0o\0m\0\x65\0/\0\x64\0t\0t\0h), @Variant(\0\0\0\v\0\0\0\x2\0\0\0\x10\0t\0\x65\0s\0t\0\x63\0\x61\0s\0\x65\0\0\0\x14\0/\0h\0o\0m\0\x65\0/\0\x64\0t\0t\0h)

      [hot_exit]
      auto_save=false
      auto_save_interval=20000
      enable=true
      EOF

      # B. GHI ĐÈ TRỰC TIẾP THEME TỐI ADWAITA-DARK VÀO XSETTINGS CỦA USER
      cat << 'EOF' > /home/dtth/.config/xfce4/xfconf/xfce-perchannel-xml/xsettings.xml
      <?xml version="1.0" encoding="UTF-8"?>
      <channel name="xsettings" version="1.0">
        <property name="Net" type="empty">
          <property name="ThemeName" type="string" value="Adwaita-dark"/>
          <property name="IconThemeName" type="string" value="Adwaita"/>
        </property>
      </channel>
      EOF

      # C. TRÍCH XUẤT WALLPAPER ĐEN TỪ REPO VÀ ÉP XFCE LOAD LÊN CHO MỌI ĐẦU MONITOR
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

      # D. BÀN GIAO QUYỀN SỞ HỮU TOÀN BỘ FILE CHO USER 'dtth' ĐỂ HỆ THỐNG ĐỌC MƯỢT MÀ
      chown -R dtth:users /home/dtth
    '';
    deps = [ "users" ];
  };

  # Vô hiệu hóa việc chờ kết nối mạng để giảm thời gian boot xuống mức tối thiểu
  systemd.services.NetworkManager-wait-online.enable = false;
}
