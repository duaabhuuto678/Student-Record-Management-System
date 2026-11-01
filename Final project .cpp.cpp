#include <windows.h>
#include <commctrl.h>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

int currentStudent = -1;

// Professional navy, white and light-grey theme.
const COLORREF NAVY = RGB(30, 58, 95);
const COLORREF LIGHT_GREY = RGB(233, 238, 245);
const COLORREF WHITE = RGB(255, 255, 255);
const COLORREF TEXT_GREY = RGB(45, 55, 70);

HBRUSH hBackgroundBrush = NULL;
HFONT hHeadingFont = NULL;
HFONT hLabelFont = NULL;
HFONT hInputFont = NULL;
HFONT hButtonFont = NULL;

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
    switch (Message) {
        case WM_CREATE: {
            hHeadingFont = CreateFontA(27, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");
            hLabelFont = CreateFontA(19, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");
            hInputFont = CreateFontA(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");
            hButtonFont = CreateFontA(17, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI");

            // Centered heading for a more professional appearance.
            HWND heading = CreateWindowA("STATIC", "STUDENT RECORD MANAGEMENT SYSTEM",
                WS_VISIBLE | WS_CHILD | SS_CENTER,
                20, 22, 720, 38, hwnd, NULL, NULL, NULL);
            SendMessage(heading, WM_SETFONT, (WPARAM)hHeadingFont, TRUE);

            // Labels and input fields: same six fields and IDs as the original.
            const char* labels[] = {"Name:", "Roll Number:", "Father's Name:", "Caste:", "District:", "GPA:"};
            for (int i = 0; i < 6; i++) {
                int y = 78 + (i * 55);
                HWND label = CreateWindowA("STATIC", labels[i], WS_VISIBLE | WS_CHILD,
                    105, y + 5, 145, 30, hwnd, NULL, NULL, NULL);
                SendMessage(label, WM_SETFONT, (WPARAM)hLabelFont, TRUE);

                HWND edit = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                    265, y, 340, 36, hwnd, (HMENU)(INT_PTR)(101 + i), NULL, NULL);
                SendMessage(edit, WM_SETFONT, (WPARAM)hInputFont, TRUE);
            }

            // Same four buttons and IDs, with a larger, consistent style.
            const char* buttonNames[] = {"Add", "Update", "Delete", "Next"};
            for (int i = 0; i < 4; i++) {
                HWND button = CreateWindowA("BUTTON", buttonNames[i],
                    WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
                    80 + (i * 155), 430, 125, 42, hwnd,
                    (HMENU)(INT_PTR)(i + 1), NULL, NULL);
                SendMessage(button, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
            }
            break;
        }

        case WM_CTLCOLORSTATIC: {
            HDC hdc = (HDC)wParam;
            SetTextColor(hdc, NAVY);
            SetBkMode(hdc, TRANSPARENT);
            return (LRESULT)hBackgroundBrush;
        }

        case WM_CTLCOLOREDIT: {
            HDC hdc = (HDC)wParam;
            SetTextColor(hdc, TEXT_GREY);
            SetBkColor(hdc, WHITE);
            static HBRUSH hEditBrush = CreateSolidBrush(WHITE);
            return (LRESULT)hEditBrush;
        }

        case WM_DRAWITEM: {
            LPDRAWITEMSTRUCT dis = (LPDRAWITEMSTRUCT)lParam;
            if (dis->CtlType == ODT_BUTTON) {
                bool pressed = (dis->itemState & ODS_SELECTED) != 0;
                RECT r = dis->rcItem;
                HBRUSH brush = CreateSolidBrush(pressed ? RGB(18, 39, 66) : NAVY);
                FillRect(dis->hDC, &r, brush);
                DeleteObject(brush);
                SetBkMode(dis->hDC, TRANSPARENT);
                SetTextColor(dis->hDC, WHITE);
                HFONT oldFont = (HFONT)SelectObject(dis->hDC, hButtonFont);
                char caption[40];
                GetWindowTextA(dis->hwndItem, caption, sizeof(caption));
                DrawTextA(dis->hDC, caption, -1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                SelectObject(dis->hDC, oldFont);
                if (dis->itemState & ODS_FOCUS) {
                    InflateRect(&r, -5, -5);
                    DrawFocusRect(dis->hDC, &r);
                }
                return TRUE;
            }
            break;
        }

	case WM_COMMAND: {
    int buttonID = LOWORD(wParam);

    if (buttonID == 1) {
    char name[100];
    char roll[100];
    char fatherName[100];
    char caste[100];
    char district[100];
    char gpa[100];

    GetWindowText(GetDlgItem(hwnd, 101), name, 100);
    GetWindowText(GetDlgItem(hwnd, 102), roll, 100);
    GetWindowText(GetDlgItem(hwnd, 103), fatherName, 100);
    GetWindowText(GetDlgItem(hwnd, 104), caste, 100);
    GetWindowText(GetDlgItem(hwnd, 105), district, 100);
    GetWindowText(GetDlgItem(hwnd, 106), gpa, 100);

    std::ofstream file("students.txt", std::ios::app);

    if (file.is_open()) {
        file << name << "," << roll << "," << fatherName << ","
             << caste << "," << district << "," << gpa << "\n";

        file.close();

        MessageBox(hwnd, "Student record saved successfully!",
                   "Success", MB_OK);
    }
    else {
        MessageBox(hwnd, "Unable to open the file!",
                   "Error", MB_OK | MB_ICONERROR);
    }
}

if (buttonID == 2) {
    char name[100];
    char roll[100];
    char fatherName[100];
    char caste[100];
    char district[100];
    char gpa[100];

    GetWindowText(GetDlgItem(hwnd, 101), name, 100);
    GetWindowText(GetDlgItem(hwnd, 102), roll, 100);
    GetWindowText(GetDlgItem(hwnd, 103), fatherName, 100);
    GetWindowText(GetDlgItem(hwnd, 104), caste, 100);
    GetWindowText(GetDlgItem(hwnd, 105), district, 100);
    GetWindowText(GetDlgItem(hwnd, 106), gpa, 100);

    if (strlen(roll) == 0) {
        MessageBox(hwnd, "Please enter the Roll Number.",
                   "Warning", MB_OK | MB_ICONWARNING);
    } else {
        std::ifstream file("students.txt");
        std::vector<std::string> records;
        std::string line;
        bool found = false;

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string savedName, savedRoll;

            std::getline(ss, savedName, ',');
            std::getline(ss, savedRoll, ',');

            if (savedRoll == roll) {
                line = std::string(name) + "," + roll + "," +
                       fatherName + "," + caste + "," +
                       district + "," + gpa;
                found = true;
            }

            records.push_back(line);
        }

        file.close();

        if (found) {
            std::ofstream outFile("students.txt", std::ios::trunc);

            for (const std::string& record : records) {
                outFile << record << "\n";
            }

            outFile.close();

            MessageBox(hwnd, "Student record updated successfully!",
                       "Success", MB_OK);
        } else {
            MessageBox(hwnd, "Roll Number not found!",
                       "Not Found", MB_OK | MB_ICONWARNING);
        }
    }
}

if (buttonID == 3) {
    char roll[100];
    GetWindowText(GetDlgItem(hwnd, 102), roll, 100);

    if (strlen(roll) == 0) {
        MessageBox(hwnd, "Please enter the Roll Number.",
                   "Warning", MB_OK | MB_ICONWARNING);
    } else {
        std::ifstream file("students.txt");
        std::vector<std::string> records;
        std::string line;
        bool found = false;

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string savedName, savedRoll;

            std::getline(ss, savedName, ',');
            std::getline(ss, savedRoll, ',');

            if (savedRoll == roll) {
                found = true;
            } else {
                records.push_back(line);
            }
        }

        file.close();

        if (found) {
            std::ofstream outFile("students.txt", std::ios::trunc);

            for (const std::string& record : records) {
                outFile << record << "\n";
            }

            outFile.close();

            MessageBox(hwnd, "Student record deleted successfully!",
                       "Success", MB_OK);
        } else {
            MessageBox(hwnd, "Roll Number not found!",
                       "Not Found", MB_OK | MB_ICONWARNING);
        }
    }
}

if (buttonID == 4) {
    std::ifstream file("students.txt");
    std::string line;
    int index = 0;
    bool found = false;

    while (std::getline(file, line)) {
        if (index == currentStudent + 1) {
            std::stringstream ss(line);
            std::string name, roll, fatherName;
            std::string caste, district, gpa;

            std::getline(ss, name, ',');
            std::getline(ss, roll, ',');
            std::getline(ss, fatherName, ',');
            std::getline(ss, caste, ',');
            std::getline(ss, district, ',');
            std::getline(ss, gpa, ',');

            SetWindowText(GetDlgItem(hwnd, 101), name.c_str());
            SetWindowText(GetDlgItem(hwnd, 102), roll.c_str());
            SetWindowText(GetDlgItem(hwnd, 103), fatherName.c_str());
            SetWindowText(GetDlgItem(hwnd, 104), caste.c_str());
            SetWindowText(GetDlgItem(hwnd, 105), district.c_str());
            SetWindowText(GetDlgItem(hwnd, 106), gpa.c_str());

            currentStudent = index;
            found = true;
            break;
        }

        index++;
    }
        file.close();

    if (!found) {
        MessageBox(hwnd, "No more student records!",
                   "Information", MB_OK | MB_ICONINFORMATION);
    }}
break;
}
		

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, Message, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEXA wc;
    HWND hwnd;
    MSG msg;

    hBackgroundBrush = CreateSolidBrush(LIGHT_GREY);
    ZeroMemory(&wc, sizeof(wc));
    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = hBackgroundBrush;
    wc.lpszClassName = "WindowClass";
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassExA(&wc)) {
        MessageBoxA(NULL, "Window Registration Failed!", "Error", MB_ICONERROR | MB_OK);
        return 0;
    }

    hwnd = CreateWindowExA(0, "WindowClass", "Student Record Management System",
        WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        760, 565, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        MessageBoxA(NULL, "Window Creation Failed!", "Error", MB_ICONERROR | MB_OK);
        return 0;
    }

    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (hHeadingFont) DeleteObject(hHeadingFont);
    if (hLabelFont) DeleteObject(hLabelFont);
    if (hInputFont) DeleteObject(hInputFont);
    if (hButtonFont) DeleteObject(hButtonFont);
    if (hBackgroundBrush) DeleteObject(hBackgroundBrush);
    return (int)msg.wParam;
}
