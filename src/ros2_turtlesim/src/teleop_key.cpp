/* Gunakan kode berikut untuk membaca input dari keyboard tanpa menunggu tombol enter ditekan.
Kode ini menggunakan fungsi `read` dari header `<unistd.h>` untuk membaca karakter secara langsung dari standar input (keyboard).
Pastikan untuk mengatur terminal ke mode raw agar karakter dapat dibaca segera setelah ditekan, tanpa buffering.

read(STDIN_FILENO, &c, 1) akan membaca satu karakter dari keyboard dan menyimpannya dalam variabel `c`.
-1 akan dikembalikan jika terjadi kesalahan saat membaca,
0 akan dikembalikan jika tidak ada karakter yang tersedia untuk dibaca (misalnya, jika tidak ada input).
1 akan dikembalikan jika berhasil membaca satu karakter.

char c;
int n = read(STDIN_FILENO, &c, 1);
*/

#include <cstdio>
#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include <geometry_msgs/msg/twist.hpp>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

class my_teleop_key : public rclcpp::Node {
  public:
   my_teleop_key() : Node("my_telkey"){
    // edit disini

    // jangan dihapus
    setTerminalRawMode(true);
   }

   ~my_teleop_key() {
    setTerminalRawMode(false);
   }

  void setTerminalRawMode(bool enable) {
#ifdef _WIN32
    // Windows implementation
    static HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    static DWORD dwMode = 0;
    
    if (enable) {
        GetConsoleMode(hStdin, &dwMode);
        DWORD dwNewMode = dwMode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
        SetConsoleMode(hStdin, dwNewMode);
    } else {
        SetConsoleMode(hStdin, dwMode);
    }
#else
    // Unix/Linux implementation
    if (enable) {
        // Get current terminal settings for Standard Input (file descriptor 0)
        if (tcgetattr(STDIN_FILENO, &oldt) == -1) {
            perror("tcgetattr");
            return;
        }
        newt = oldt;
        
        // Disable canonical mode (ICANON) and local echo (ECHO)
        newt.c_lflag &= ~(ICANON | ECHO);
        newt.c_cc[VMIN] = 0;
        newt.c_cc[VTIME] = 0;
        
        // Apply settings immediately
        if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) == -1) {
            perror("tcsetattr");
        }
    } else {
        // Restore original terminal configurations
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
#endif
  }

  private:
#ifdef _WIN32
    // Windows-specific members if needed
#else
    struct termios oldt;
    struct termios newt;
#endif
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<my_teleop_key>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  
  return 0;
}
