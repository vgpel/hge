#include "..\..\include\hge.h"
#include "..\..\include\hgeEditCN.h"
#include "..\..\include\hgeFontCN.h"
#include "..\..\include\hgeFlashPlayer.h"
#include "..\..\include\hgeFlashSprite.h"

HGE*		g_pHGE		= NULL;
GfxFont*	g_pGfxFont0	= NULL;	// 中文显示
GfxEdit*	g_pGfxEdit0	= NULL;	// 中文输入
GfxFont*	g_pGfxFont1	= NULL;	// 中文显示
GfxEdit*	g_pGfxEdit1	= NULL;	// 中文输入
char*       g_lpString0 = "数字只读框";
char*       g_lpString1 = "支持光标定位、选字、全选、复制、粘贴、剪切等操作";

FlashSprite* g_pFlashSprite = NULL;

void Init()
{
	// 创建字体
	g_pGfxFont0 = new GfxFont("宋体",15,FALSE,FALSE,FALSE);// 宋书，非粗体，非斜体，非平滑
	g_pGfxFont0->SetColor(0xFF00FFFF);		// 设置像素字体颜色

	// 创建编辑框
	g_pGfxEdit0 = new GfxEdit(100,0xFFFF8822,"宋体",15,FALSE,FALSE,FALSE);// 编辑框字体指定
	g_pGfxEdit0->InsertCookie("123");
	//g_pGfxEdit0->ShowOuterFrame(false);
	g_pGfxEdit0->SetNumOnly(true);

	// 创建字体
	g_pGfxFont1 = new GfxFont("楷体",22,FALSE,FALSE,TRUE);// 楷体，非粗体，非斜体，平滑
	g_pGfxFont1->SetColor(0xFF00FFFF);		// 设置像素字体颜色

	// 创建编辑框
	g_pGfxEdit1 = new GfxEdit(750,0xFFFFFF00,"楷体",22,FALSE,FALSE,TRUE);// 楷体，非粗体，非斜体，平滑
	g_pGfxEdit1->InsertCookie("支持 BackSpace、Delete、Left、Right、Home、End与Tab键操作。");//

    // 创建flash精灵
    g_pFlashSprite = new FlashSprite;
    g_pFlashSprite->StartFlash("FLASH_001.swf",720,576);
}

void Done()
{
	if (g_pGfxFont0) delete g_pGfxFont0;// 销毁字体
	if (g_pGfxEdit0) delete g_pGfxEdit0;// 销毁编辑框
	if (g_pGfxFont1) delete g_pGfxFont1;// 销毁字体
	if (g_pGfxEdit1) delete g_pGfxEdit1;// 销毁编辑框
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

    // 更新flash精灵帧
    g_pFlashSprite->Update(dt);

	return false;
}

bool RenderFunc()
{
	g_pHGE->Gfx_Clear(0);
	g_pHGE->Gfx_BeginScene();

	// 从编辑框中获取字符串内容

	const char* lpString2 = "数字只读框";
	const char* lpString3 = "HGE 中文显示与输入编辑框解决新方案及Flash支持";
	const char* lpString4 = "支持光标定位、选字、全选、复制、粘贴、剪切等操作。";

	// 渲染中文
    g_pGfxFont0->Print(100,100,g_lpString0);
    g_pGfxFont1->Print(10,200,g_lpString1);
	g_pGfxFont0->Print(10,100,lpString2);
	g_pGfxFont0->Print(10,25,lpString3);
	g_pGfxFont0->Print(10,52,lpString4);

	// 渲染编辑框
	g_pGfxEdit0->Render(10,130);
	g_pGfxEdit1->Render(10,250);

    // 绘制flash到指定坐标
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
	g_pHGE->System_SetState(HGE_TITLE, "g_pHGE 中文显示与输入编辑框解决新方案及Flash支持");
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
