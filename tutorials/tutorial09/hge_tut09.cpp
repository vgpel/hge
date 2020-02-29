#include "..\..\include\hge.h"
#include "..\..\include\hgeEditCN.h"
#include "..\..\include\hgeFontCN.h"
#include "..\..\include\hgeFlashPlayer.h"
#include "..\..\include\hgeFlashSprite.h"

HGE*		g_pHGE		= NULL;
GfxFont*	g_pGfxFont0	= NULL;	// ������ʾ
GfxEdit*	g_pGfxEdit0	= NULL;	// ��������
GfxFont*	g_pGfxFont1	= NULL;	// ������ʾ
GfxEdit*	g_pGfxEdit1	= NULL;	// ��������
char*       g_lpString0 = "����ֻ����";
char*       g_lpString1 = "֧�ֹ�궨λ��ѡ�֡�ȫѡ�����ơ�ճ�������еȲ���";

FlashSprite* g_pFlashSprite = NULL;

void Init()
{
	// ��������
	g_pGfxFont0 = new GfxFont("����",15,FALSE,FALSE,FALSE);// ���飬�Ǵ��壬��б�壬��ƽ��
	g_pGfxFont0->SetColor(0xFF00FFFF);		// ��������������ɫ

	// �����༭��
	g_pGfxEdit0 = new GfxEdit(100,0xFFFF8822,"����",15,FALSE,FALSE,FALSE);// �༭������ָ��
	g_pGfxEdit0->InsertCookie("123");
	//g_pGfxEdit0->ShowOuterFrame(false);
	g_pGfxEdit0->SetNumOnly(true);

	// ��������
	g_pGfxFont1 = new GfxFont("����",22,FALSE,FALSE,TRUE);// ���壬�Ǵ��壬��б�壬ƽ��
	g_pGfxFont1->SetColor(0xFF00FFFF);		// ��������������ɫ

	// �����༭��
	g_pGfxEdit1 = new GfxEdit(750,0xFFFFFF00,"����",22,FALSE,FALSE,TRUE);// ���壬�Ǵ��壬��б�壬ƽ��
	g_pGfxEdit1->InsertCookie("֧�� BackSpace��Delete��Left��Right��Home��End��Tab��������");//

    // ����flash����
    g_pFlashSprite = new FlashSprite;
    g_pFlashSprite->StartFlash("FLASH_001.swf",720,576);
}

void Done()
{
	if (g_pGfxFont0) delete g_pGfxFont0;// ��������
	if (g_pGfxEdit0) delete g_pGfxEdit0;// ���ٱ༭��
	if (g_pGfxFont1) delete g_pGfxFont1;// ��������
	if (g_pGfxEdit1) delete g_pGfxEdit1;// ���ٱ༭��
    if (g_pFlashSprite) delete g_pFlashSprite;
}


bool FrameFunc()
{
	float dt=g_pHGE->Timer_GetDelta();

    if (g_pHGE->Input_KeyDown(HGEK_ENTER))
    {
        if (g_pGfxEdit0->IsFocus())
        {
            g_lpString0 = (char*)g_pGfxEdit0->GetCookie();
            g_pGfxEdit0->ClearCookie();
        }
        if (g_pGfxEdit1->IsFocus())
        {
            g_lpString1 = (char*)g_pGfxEdit1->GetCookie();
            g_pGfxEdit1->ClearCookie();
        }
    }

	g_pGfxEdit0->Updata(dt);
	g_pGfxEdit1->Updata(dt);

    // ����flash����֡
    g_pFlashSprite->Update(dt);

	return false;
}

bool RenderFunc()
{
	g_pHGE->Gfx_Clear(0);
	g_pHGE->Gfx_BeginScene();

	// �ӱ༭���л�ȡ�ַ�������

	const char* lpString2 = "����ֻ����";
	const char* lpString3 = "HGE ������ʾ������༭�����·�����Flash֧��";
	const char* lpString4 = "֧�ֹ�궨λ��ѡ�֡�ȫѡ�����ơ�ճ�������еȲ�����";

	// ��Ⱦ����
    g_pGfxFont0->Print(100,100,g_lpString0);
    g_pGfxFont1->Print(10,200,g_lpString1);
	g_pGfxFont0->Print(10,100,lpString2);
	g_pGfxFont0->Print(10,25,lpString3);
	g_pGfxFont0->Print(10,52,lpString4);

	// ��Ⱦ�༭��
	g_pGfxEdit0->Render(10,130);
	g_pGfxEdit1->Render(10,250);

    // ����flash��ָ������
    g_pFlashSprite->Render(10,10);

	g_pHGE->Gfx_EndScene();

	return false;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	g_pHGE = hgeCreate(HGE_VERSION);

	g_pHGE->System_SetState(HGE_INIFILE, "hgedemo.ini");
	g_pHGE->System_SetState(HGE_LOGFILE, "hgedemo.log");
	g_pHGE->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	g_pHGE->System_SetState(HGE_RENDERFUNC, RenderFunc);
	g_pHGE->System_SetState(HGE_TITLE, "g_pHGE ������ʾ������༭�����·�����Flash֧��");
	g_pHGE->System_SetState(HGE_SCREENWIDTH, 800);
	g_pHGE->System_SetState(HGE_SCREENHEIGHT, 600);
	g_pHGE->System_SetState(HGE_SCREENBPP, 32);
	g_pHGE->System_SetState(HGE_WINDOWED, true);
	g_pHGE->System_SetState(HGE_HIDEMOUSE, false);
	g_pHGE->System_SetState(HGE_USESOUND, false);

	if(g_pHGE->System_Initiate())
	{
		Init();
		g_pHGE->System_Start();
	}
	else MessageBox(NULL, g_pHGE->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
	Done();

	g_pHGE->System_Shutdown();
	g_pHGE->Release();
	return 0;
}
