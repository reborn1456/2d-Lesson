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
	hWnd = CreateWindow("Test", "TITLE", WS_OVERLAPPEDWINDOW, 200, 100, 500, 400, NULL, NULL, hInst, NULL);

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
	static int x = 0;
	int mx, my;

	switch (uMsg)
	{
	case WM_DESTROY:                                                  // �E�B���h�E���j�����ꂽ
		PostQuitMessage(0);
		break;
		// =============================== New added in this lesson =====================================
	case WM_KEYDOWN:
		hdc = GetDC(hWnd);
		if (wParam == VK_SPACE)
		{
			SetTextColor(hdc, RGB(255, 255, 0));
			SetBkColor(hdc, RGB(0, 0, 255));
			TextOut(hdc, x, 50, "����ɂ���", 10);
			x += 1;
		}
		ReleaseDC(hWnd, hdc);
		break;
	case WM_LBUTTONDOWN:
		hdc = GetDC(hWnd);
		mx = LOWORD(lParam);   //���ʂ̂P�U�������
		my = HIWORD(lParam);   //��ʂ̂P�U�������
		SetPixel(hdc, mx, my, RGB(255, 255, 255));
		ReleaseDC(hWnd, hdc);
		break;
		// =============================== New added in this lesson =====================================
	default:                                                          // ����ȊO�́i���̏��������Ȃ��j���b�Z�[�W�̂Ƃ�
		return DefWindowProc(hWnd, uMsg, wParam, lParam);             // OS�ɏ������܂�����
	}

	return 0;
}