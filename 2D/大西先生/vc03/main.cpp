// �v���W�F�N�g���Fskelton
// �t�@�C�����Fmain.cpp
// �X�P���g���i���g�݁j�v���O����
// =================================== �w�b�_�t�@�C���̃C���N���[�h ===================================
#include <windows.h>




// =================================== �֐��v���g�^�C�v�錾 ===================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);




// =================================== �G���g���|�C���g�i�o���_�j ===================================
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;           // ���b�Z�[�W�i�\���́j�ϐ���錾
	WNDCLASS wc;       // �E�B���h�E�N���X�i�\���́j�ϐ���錾
	HWND hWnd;         // �E�B���h�E�n���h���ϐ���錾

	// =================================== �E�B���h�E����� ===================================
	// �@�E�B���h�E�N���X�i���E�B���h�E�̐݌v�}�j�̓o�^�\��
	wc.style = CS_HREDRAW | CS_VREDRAW;                                 // �X�^�C��
	wc.lpfnWndProc = WndProc;                                           // ���d�v���E�B���h�E�v���V�[�W�����i�A�h���X�j
	wc.cbClsExtra = 0;                                                  // �E�B���h�E�N���X�̕ێ�����f�[�^�T�C�Y
	wc.cbWndExtra = 0;                                                  // �E�B���h�E�̕ێ�����f�[�^�T�C�Y
	wc.hInstance = hInst;                                               // ���d�v���C���X�^���X�n���h��
	wc.hIcon = NULL;                                                    // �A�C�R��
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);                           // �J�[�\���i�}�E�X�|�C���^�j
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);             // �E�B���h�E�̔w�i�F
	wc.lpszMenuName = (LPSTR)NULL;                                      // ���j���[
	wc.lpszClassName = "Test";                                          // ���d�v���E�B���h�E�N���X�̖��O
	RegisterClass(&wc);                                                 // �o�^����

	// �A�E�B���h�E�N���X�ɏ]���E�B���h�E���i��������Ɂj���
	hWnd = CreateWindow("Test", "�f�o�C�X�R���e�L�X�g", WS_OVERLAPPEDWINDOW, 10, 50, 1280, 720, NULL, NULL, hInst, NULL);

	ShowWindow(hWnd, nCmdShow);        // �B�E�B���h�E��\������
	UpdateWindow(hWnd);                // �C�N���C�A���g�̈��\���X�V����

	// =================================== ���b�Z�[�W���[�v ===================================
	while (GetMessage(&msg, NULL, 0, 0))        // �D���b�Z�[�W���擾����
	{
		TranslateMessage(&msg);                 // �E���b�Z�[�W��ϊ�����
		DispatchMessage(&msg);                  // �F�E�B���h�E�v���V�[�W���փ��b�Z�[�W�𑗂�
	}

	return (int)msg.wParam;                     // �I��
}

// =================================== �E�B���h�E�v���V�[�W�� ===================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	char str[50];
	static char work[100];   // static = remembers what is inside
	static int x = 0, y = 50;
	int mouseX = 0, mouseY = 0;
	int i = 1200, j = 720;
	POINT Mouse;

	switch (uMsg)
	{
	case WM_DESTROY:                                                  // �E�B���h�E���j�����ꂽ
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		hdc = GetDC(hWnd);

		if (wParam == 'C')
		{
			PatBlt(hdc, 0, 0, 1280, 720, BLACKNESS);
		}

		SetTextColor(hdc, RGB(255, 0, 0));
		SetBkColor(hdc, RGB(255, 255, 0));
		TextOut(hdc, 0, 0, "����ɂ���", 10);
		ReleaseDC(hWnd, hdc);
		break;
	case WM_CHAR:
		if (wParam == '\r')   // ���s�L�[�̎�
		{
			x = 0;
			y += 16;
		}
		else
		{
			hdc = GetDC(hWnd);
			//wsprintf(str, "%c�����͂��ꂽ", wParam);
			//TextOut(hdc, 0, 0, str, 14);
			wsprintf(str, "%c", wParam);
			//		strcat_s(work, str);
			TextOut(hdc, x, y, str, strlen(str));
			x += 10;
			ReleaseDC(hWnd, hdc);
		}
		break;
	case WM_LBUTTONDOWN:
		GetCursorPos(&Mouse);
		ScreenToClient(hWnd, &Mouse);

		hdc = GetDC(hWnd);
		SetTextColor(hdc, RGB(255, 255, 255));
		SetBkColor(hdc, RGB(0, 0, 0));
		TextOut(hdc, Mouse.x, Mouse.y, "�D", 2);
		ReleaseDC(hWnd, hdc);
		break;
	case WM_RBUTTONUP:
		hdc = GetDC(hWnd);
		SetTextColor(hdc, RGB(0, 255, 255));
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, 5, 5, "1106046", 7);
		ReleaseDC(hWnd, hdc);
		break;
	case WM_MOUSEMOVE:
		GetCursorPos(&Mouse);
		ScreenToClient(hWnd, &Mouse);
		//mouseX = LOWORD(lParam);   //���ʂ̂P�U�������
		//mouseY = HIWORD(lParam);   //��ʂ̂P�U�������

		hdc = GetDC(hWnd);
		//SetPixel(hdc, Mouse.x, Mouse.y, RGB(0, 0, 0));
		while (i > 80 && j > 20)
		{
			Ellipse(hdc, 300, 100, 1200, 700);
		}
		ReleaseDC(hWnd, hdc);
		break;
	default:                                                          // ����ȊO�́i���̏��������Ȃ��j���b�Z�[�W�̂Ƃ�
		return DefWindowProc(hWnd, uMsg, wParam, lParam);             // OS�ɏ������܂�����
	}

	return 0;
}