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
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);             // �E�B���h�E�̔w�i�F
//	wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(250, 0, 0));        // RGB�ŃE�B���h�E�̔w�i�F��ς���, Release���ĂȂ����炠��܂�g��Ȃ���������
	wc.lpszMenuName = (LPSTR)NULL;                                      // ���j���[
	wc.lpszClassName = "Test";                                          // ���d�v���E�B���h�E�N���X�̖��O
	RegisterClass(&wc);                                                 // �o�^����

	// �A�E�B���h�E�N���X�ɏ]���E�B���h�E���i��������Ɂj���
	hWnd = CreateWindow("Test", "�E�B���h�E�v���V�[�W���̊w�K", WS_OVERLAPPEDWINDOW, 10, 50, 800, 600, NULL, NULL, hInst, NULL);

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
	int m = 0;
	char str[50];
	int mouseX = 0, mouseY = 0;
	POINT Mouse;

	switch (uMsg)
	{
	case WM_CREATE:
		MessageBeep(MB_ICONASTERISK);
		while (1)   // �������[�v
		{
			m = MessageBox(hWnd, "A Window Has Been Created", "Message Box", MB_ABORTRETRYIGNORE | MB_DEFBUTTON3 | MB_ICONEXCLAMATION);   // ���b�Z�[�W�{�b�N�X��\������
			if (m == IDABORT)   // ���~����������
			{
				DestroyWindow(hWnd);   // �E�B���h�E���j�������
				return 0;
			}
			if (m == IDIGNORE)   // ��������������
				break;   // ���[�v�𔲂���
		}
		break;

	case WM_MOUSEMOVE:
		for (int i = 0; i < 10; i++)
		{
			FlashWindow(hWnd, TRUE);  // if false then it just simply does't flash
		}
		break;

	case WM_KEYDOWN:
		switch (wParam) 
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
		case 'B':
			MessageBeep(MB_ICONASTERISK);
			break;
		}
		break;

	case WM_KEYUP:
		switch (wParam)
		{
		case 'A':
			FlashWindow(hWnd, TRUE);  // if false then it just simply does't flash
			break;
		}
		break;

	case WM_CHAR:
//		str[0] = wParam;
//		str[1] = '\0';
		wsprintf(str, "%c", wParam);
		MessageBox(hWnd, str, TEXT("Message Box"), MB_OK);
		break;

	case WM_LBUTTONUP:
//		mouseX = lParam & 0x0000FFFF;
		mouseX = LOWORD(lParam);   //���ʂ̂P�U�������
//		mouseY = (lParam >> 16) & 0x0000FFFF;   // ��ڂ̕��@�B�V�t�g���Ă��灕�i�A���h�j
//		mouseY = (lParam & 0xFFFF0000) >> 16;   // ��ڂ̕��@�B���i�A���h�j����V�t�g����
		mouseY = HIWORD(lParam);   //��ʂ̂P�U�������
		wsprintf(str, "�}�E�X���W( %d, %d )", mouseX, mouseY);
		//GetCursorPos(&Mouse);
		//ScreenToClient(hWnd, &Mouse);
		//wsprintf(str, "�}�E�X���W( %d, %d )", Mouse.x, Mouse.y);
		SetWindowText(hWnd, str);
		break;

	case WM_CLOSE:
		if (IDYES == MessageBox(hWnd, TEXT("Are you sure you want to quit this program?"), TEXT("Confirmation"), MB_YESNO | MB_DEFBUTTON2))
		{
			DestroyWindow(hWnd);
			return 0;
		}
		break;

	case WM_RBUTTONDOWN:
		if (IDYES == MessageBox(hWnd, TEXT("Are you sure you want to quit this program?"), TEXT("Confirmation"), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2))
		{
			DestroyWindow(hWnd);
			return 0;
		}
		break;

	case WM_DESTROY:                                                  // �E�B���h�E���j�����ꂽ
		PostQuitMessage(0);
		return 0;

	default:                                                          // ����ȊO�́i���̏��������Ȃ��j���b�Z�[�W�̂Ƃ�
		return DefWindowProc(hWnd, uMsg, wParam, lParam);             // OS�ɏ������܂�����
	}

	return 0;
}