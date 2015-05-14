#include <ddraw.h>
#include <assert.h>

#include "..\_MSA_Utils\MSA_Utils.h"
#include "MSA_DDraw.h"

#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "ddraw.lib ")



void MSA_DDRAW::Init()
{
	CLEARME;
}


void MSA_DDRAW::Destroy()
{
	ReleaseSurfaces();
	REL(pDD4);
}


MSA_DDRAW::MSA_DDRAW()
{
	Init();
	InitDDraw();
}


MSA_DDRAW::MSA_DDRAW(MSA_WINAPP* tApp)
{
	Init();
	TheApp=tApp;
	InitDDraw();
}


MSA_DDRAW::~MSA_DDRAW()
{
	Destroy();
}


HRESULT MSA_DDRAW::InitDDraw()
{
    LPDIRECTDRAW pDD;
	
	_DDErrorCheck( DirectDrawCreate (NULL, &pDD, NULL)							, "DirectDrawCreate FAILED");
	_DDErrorCheck( pDD->QueryInterface	(IID_IDirectDraw4, (LPVOID *) &pDD4)	, "QueryInterface FAILED"  );
    return DD_OK;
}


HRESULT MSA_DDRAW::InitSurfaces(bool tbWindow, long tScrX, long tScrY, long tBitDepth)
{
    if(TheApp) {
		DDSCAPS2            ddscaps;
		LPDIRECTDRAWCLIPPER pClipper;
		bWindow=tbWindow;

		ReleaseSurfaces();
		if (bWindow) {
			_DDErrorCheck(pDD4->SetCooperativeLevel(TheApp->hWnd, DDSCL_NORMAL), "SetCooperativeLevel FAILED");

			GetClientRect(TheApp->hWnd, &TheApp->rcViewport);
    		GetClientRect(TheApp->hWnd, &TheApp->rcScreen);
    		ClientToScreen(TheApp->hWnd, (POINT*)&TheApp->rcScreen.left);
    		ClientToScreen(TheApp->hWnd, (POINT*)&TheApp->rcScreen.right);

			ZeroMemory(&ddsd2,sizeof(ddsd2));
			ddsd2.dwSize = sizeof(ddsd2);
			ddsd2.dwFlags = DDSD_CAPS;
			ddsd2.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
			_DDErrorCheck(pDD4->CreateSurface(&ddsd2, &pDDS4_Prim, NULL), "Create Primary Surface FAILED");
        
			ddsd2.dwFlags        = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
			ddsd2.dwWidth        = tScrX;
			ddsd2.dwHeight       = tScrY;
			ddsd2.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
			_DDErrorCheck(pDD4->CreateSurface(&ddsd2, &pDDS4_Back, NULL), "Create Flipping Surface FAILED");
		}
		else
		{
			_DDErrorCheck(pDD4->SetCooperativeLevel(TheApp->hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN), "SetCooperativeLevel FAILED");
			_DDErrorCheck(pDD4->SetDisplayMode( tScrX, tScrY, tBitDepth, 0, 0), "SetDisplayMode FAILED");
       		
			SetRect(&TheApp->rcViewport, 0, 0, tScrX, tScrY );
    		memcpy(&TheApp->rcScreen, &TheApp->rcViewport, sizeof(RECT) );
        
			ZeroMemory(&ddsd2,sizeof(ddsd2));
			ddsd2.dwSize = sizeof(ddsd2);
			ddsd2.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
			ddsd2.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
			ddsd2.dwBackBufferCount = 1;
			_DDErrorCheck(pDD4->CreateSurface( &ddsd2, &pDDS4_Prim, NULL), "Create Primary Surface FAILED");
			ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
			_DDErrorCheck(pDDS4_Prim->GetAttachedSurface(&ddscaps, &pDDS4_Back), "GetAttachedSurface FAILED");
		}
		
		_DDErrorCheck(pDD4->CreateClipper(0, &pClipper, NULL), "CreateClipper FAILED");
		pClipper->SetHWnd(0, TheApp->hWnd);
		pDDS4_Prim->SetClipper(pClipper);
		REL(pClipper);

		if(InitExtraSurfaces) _DDErrorCheck( InitExtraSurfaces(), "Could not initialize Surfaces");

		return DD_OK;
	}
	return DD_OK+1;
}


void MSA_DDRAW::SetInitFunc(HRESULT (*tInitExtraSurfaces)())
{
	InitExtraSurfaces=tInitExtraSurfaces;
}


HRESULT MSA_DDRAW::ChangeCoopLevel()
{
	TheApp->bActive=false;
	if(bWindow) GetWindowRect(TheApp->hWnd, &TheApp->rcWindow);
	bWindow = !bWindow;
	ReleaseSurfaces();

    if (bWindow) { 
		SetWindowPos(TheApp->hWnd, HWND_NOTOPMOST, TheApp->rcWindow.left, TheApp->rcWindow.top,
                     (TheApp->rcWindow.right - TheApp->rcWindow.left), 
                     (TheApp->rcWindow.bottom - TheApp->rcWindow.top), SWP_SHOWWINDOW );
    }

	_DDErrorCheck( InitSurfaces(bWindow), "Could not initialize Main Surfaces");
	TheApp->bActive=true;

	return DD_OK;
}


void MSA_DDRAW::ReleaseSurfaces() 
{
    if (pDD4) {
        pDD4->SetCooperativeLevel(TheApp->hWnd, DDSCL_NORMAL);
		REL(pDDS4_Back);
		REL(pDDS4_Prim);
    }
}

LPDIRECTDRAWSURFACE4 MSA_DDRAW::GetSurface(int s)
{
	return (s==DD_PRIMARY) ? pDDS4_Prim : pDDS4_Back;
}


void MSA_DDRAW::Flip()
{
	if (bWindow) pDDS4_Prim->Blt(&TheApp->rcScreen, pDDS4_Back,&TheApp->rcViewport, DDBLT_ASYNC ,NULL);
	else		 pDDS4_Prim->Flip( NULL, DDFLIP_NOVSYNC );
//	else		 pDDS4_Prim->Flip( NULL, 0 );
}


void MSA_DDRAW::FlipWait()
{
	HRESULT hRet;
	while(1) {
		if (bWindow) hRet = pDDS4_Prim->Blt(&TheApp->rcScreen, pDDS4_Back,&TheApp->rcViewport, DDBLT_WAIT ,NULL);
		else		 hRet = pDDS4_Prim->Flip( NULL, DDFLIP_NOVSYNC );
		Error("Flipping Error",hRet);
		if (hRet == DD_OK ) return;
		if (hRet == DDERR_SURFACELOST ) {
			hRet = pDDS4_Prim->Restore();
			if (hRet != DD_OK) return;
		}
		if (hRet != DDERR_WASSTILLDRAWING) return;
	}
}


void MSA_DDRAW::Clear(int iColor)
{
	DDBLTFX ddbltfx;
	ddbltfx.dwSize = sizeof(DDBLTFX);
    ddbltfx.dwFillColor = iColor;
    pDDS4_Back->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);
}


void MSA_DDRAW::AfterWinPaint()
{
	HRESULT hRet;
	if (bWindow) {
        while (1) {
            hRet = pDDS4_Prim->Blt(&TheApp->rcScreen, pDDS4_Back,&TheApp->rcViewport, DDBLT_WAIT,NULL);
			Error("After Paint",hRet);
            if (hRet == DD_OK) return;
            if (hRet == DDERR_SURFACELOST) {
                hRet = pDDS4_Prim->Restore();
                if (hRet != DD_OK ) return;
            }
            if (hRet != DDERR_WASSTILLDRAWING) return;
        }
	}
}


void MSA_DDRAW::Error(LPCSTR szTitle, HRESULT hRet) 
{
#ifdef _DDError
	
	switch (hRet) {
		case DD_OK:break;
//			TheApp->ErrorBox(szTitle, "The request completed successfully. ",true);
		case DDERR_ALREADYINITIALIZED :
			TheApp->ErrorBox(szTitle, "The object has already been initialized. ",true);
		case DDERR_BLTFASTCANTCLIP :
			TheApp->ErrorBox(szTitle, "A DirectDrawClipper object is attached to a source surface that has passed into a call to the IDirectDrawSurface4::BltFast method. ",true);
		case DDERR_CANNOTATTACHSURFACE :
			TheApp->ErrorBox(szTitle, "A surface cannot be attached to another requested surface. ",true);
		case DDERR_CANNOTDETACHSURFACE :
			TheApp->ErrorBox(szTitle, "A surface cannot be detached from another requested surface. ",true);
		case DDERR_CANTCREATEDC :
			TheApp->ErrorBox(szTitle, "Windows can not create any more device contexts (DCs), or a DC was requested for a palette-indexed surface when the surface had no palette and the display mode was not palette-indexed (in this case DirectDraw cannot select a proper palette into the DC). ",true);
		case DDERR_CANTDUPLICATE :
			TheApp->ErrorBox(szTitle, "Primary and 3-D surfaces, or surfaces that are implicitly created, cannot be duplicated. ",true);
		case DDERR_CANTLOCKSURFACE :
			TheApp->ErrorBox(szTitle, "Access to this surface is refused because an attempt was made to lock the primary surface without DCI support. ",true);
		case DDERR_CANTPAGELOCK :
			TheApp->ErrorBox(szTitle, "An attempt to page lock a surface failed. Page lock will not work on a display-memory surface or an emulated primary surface. ",true);
		case DDERR_CANTPAGEUNLOCK :
			TheApp->ErrorBox(szTitle, "An attempt to page unlock a surface failed. Page unlock will not work on a display-memory surface or an emulated primary surface. ",true);
		case DDERR_CLIPPERISUSINGHWND :
			TheApp->ErrorBox(szTitle, "An attempt was made to set a clip list for a DirectDrawClipper object that is already monitoring a window handle. ",true);
		case DDERR_COLORKEYNOTSET :
			TheApp->ErrorBox(szTitle, "No source color key is specified for this operation. ",true);
		case DDERR_CURRENTLYNOTAVAIL :
			TheApp->ErrorBox(szTitle, "No support is currently available. ",true);
		case DDERR_DCALREADYCREATED :
			TheApp->ErrorBox(szTitle, "A device context (DC) has already been returned for this surface. Only one DC can be retrieved for each surface. ",true);
		case DDERR_DEVICEDOESNTOWNSURFACE :
			TheApp->ErrorBox(szTitle, "Surfaces created by one DirectDraw device cannot be used directly by another DirectDraw device. ",true);
		case DDERR_DIRECTDRAWALREADYCREATED :
			TheApp->ErrorBox(szTitle, "A DirectDraw object representing this driver has already been created for this process. ",true);
		case DDERR_EXCEPTION :
			TheApp->ErrorBox(szTitle, "An exception was encountered while performing the requested operation. ",true);
		case DDERR_EXCLUSIVEMODEALREADYSET :
			TheApp->ErrorBox(szTitle, "An attempt was made to set the cooperative level when it was already set to exclusive. ",true);
		case DDERR_EXPIRED :
			TheApp->ErrorBox(szTitle, "The data has expired and is therefore no longer valid. ",true);
		case DDERR_GENERIC :
			TheApp->ErrorBox(szTitle, "There is an undefined error condition. ",true);
		case DDERR_HEIGHTALIGN :
			TheApp->ErrorBox(szTitle, "The height of the provided rectangle is not a multiple of the required alignment. ",true);
		case DDERR_HWNDALREADYSET :
			TheApp->ErrorBox(szTitle, "The DirectDraw cooperative level window handle has already been set. It cannot be reset while the process has surfaces or palettes created. ",true);
		case DDERR_HWNDSUBCLASSED :
			TheApp->ErrorBox(szTitle, "DirectDraw is prevented from restoring state because the DirectDraw cooperative level window handle has been subclassed. ",true);
		case DDERR_IMPLICITLYCREATED :
			TheApp->ErrorBox(szTitle, "The surface cannot be restored because it is an implicitly created surface. ",true);
		case DDERR_INCOMPATIBLEPRIMARY :
			TheApp->ErrorBox(szTitle, "The primary surface creation request does not match with the existing primary surface. ",true);
		case DDERR_INVALIDCAPS :
			TheApp->ErrorBox(szTitle, "One or more of the capability bits passed to the callback function are incorrect. ",true);
		case DDERR_INVALIDCLIPLIST :
			TheApp->ErrorBox(szTitle, "DirectDraw does not support the provided clip list. ",true);
		case DDERR_INVALIDDIRECTDRAWGUID :
			TheApp->ErrorBox(szTitle, "The globally unique identifier (GUID) passed to the DirectDrawCreate function is not a valid DirectDraw driver identifier. ",true);
		case DDERR_INVALIDMODE :
			TheApp->ErrorBox(szTitle, "DirectDraw does not support the requested mode. ",true);
		case DDERR_INVALIDOBJECT :
			TheApp->ErrorBox(szTitle, "DirectDraw received a pointer that was an invalid DirectDraw object. ",true);
		case DDERR_INVALIDPARAMS :
			TheApp->ErrorBox(szTitle, "One or more of the parameters passed to the method are incorrect. ",true);
		case DDERR_INVALIDPIXELFORMAT :
			TheApp->ErrorBox(szTitle, "The pixel format was invalid as specified. ",true);
		case DDERR_INVALIDPOSITION :
			TheApp->ErrorBox(szTitle, "The position of the overlay on the destination is no longer legal. ",true);
		case DDERR_INVALIDRECT :
			TheApp->ErrorBox(szTitle, "The provided rectangle was invalid. ",true);
		case DDERR_INVALIDSTREAM :
			TheApp->ErrorBox(szTitle, "The specified stream contains invalid data. ",true);
		case DDERR_INVALIDSURFACETYPE :
			TheApp->ErrorBox(szTitle, "The requested operation could not be performed because the surface was of the wrong type. ",true);
		case DDERR_LOCKEDSURFACES :
			TheApp->ErrorBox(szTitle, "One or more surfaces are locked, causing the failure of the requested operation. ",true);
		case DDERR_MOREDATA :
			TheApp->ErrorBox(szTitle, "There is more data available than the specified buffer size can hold. ",true);
		case DDERR_NO3D :
			TheApp->ErrorBox(szTitle, "No 3-D hardware or emulation is present. ",true);
		case DDERR_NOALPHAHW :
			TheApp->ErrorBox(szTitle, "No alpha acceleration hardware is present or available, causing the failure of the requested operation. ",true);
		case DDERR_NOBLTHW :
			TheApp->ErrorBox(szTitle, "No blitter hardware is present. ",true);
		case DDERR_NOCLIPLIST :
			TheApp->ErrorBox(szTitle, "No clip list is available. ",true);
		case DDERR_NOCLIPPERATTACHED :
			TheApp->ErrorBox(szTitle, "No DirectDrawClipper object is attached to the surface object. ",true);
		case DDERR_NOCOLORCONVHW :
			TheApp->ErrorBox(szTitle, "The operation cannot be carried out because no color-conversion hardware is present or available. ",true);
		case DDERR_NOCOLORKEY :
			TheApp->ErrorBox(szTitle, "The surface does not currently have a color key. ",true);
		case DDERR_NOCOLORKEYHW :
			TheApp->ErrorBox(szTitle, "The operation cannot be carried out because there is no hardware support for the destination color key. ",true);
		case DDERR_NOCOOPERATIVELEVELSET :
			TheApp->ErrorBox(szTitle, "A create function is called without the IDirectDraw4::SetCooperativeLevel method being called. ",true);
		case DDERR_NODC :
			TheApp->ErrorBox(szTitle, "No DC has ever been created for this surface. ",true);
		case DDERR_NODDROPSHW :
			TheApp->ErrorBox(szTitle, "No DirectDraw raster operation (ROP) hardware is available. ",true);
		case DDERR_NODIRECTDRAWHW :
			TheApp->ErrorBox(szTitle, "Hardware-only DirectDraw object creation is not possible; the driver does not support any hardware. ",true);
		case DDERR_NODIRECTDRAWSUPPORT :
			TheApp->ErrorBox(szTitle, "DirectDraw support is not possible with the current display driver. ",true);
		case DDERR_NOEMULATION :
			TheApp->ErrorBox(szTitle, "Software emulation is not available. ",true);
		case DDERR_NOEXCLUSIVEMODE :
			TheApp->ErrorBox(szTitle, "The operation requires the application to have exclusive mode, but the application does not have exclusive mode. ",true);
		case DDERR_NOFLIPHW :
			TheApp->ErrorBox(szTitle, "Flipping visible surfaces is not supported. ",true);
		case DDERR_NOFOCUSWINDOW :
			TheApp->ErrorBox(szTitle, "An attempt was made to create or set a device window without first setting the focus window. ",true);
		case DDERR_NOGDI :
			TheApp->ErrorBox(szTitle, "No GDI is present. ",true);
		case DDERR_NOHWND :
			TheApp->ErrorBox(szTitle, "Clipper notification requires a window handle, or no window handle has been previously set as the cooperative level window handle. ",true);
		case DDERR_NOMIPMAPHW :
			TheApp->ErrorBox(szTitle, "The operation cannot be carried out because no mipmap capable texture mapping hardware is present or available. ",true);
		case DDERR_NOMIRRORHW :
			TheApp->ErrorBox(szTitle, "The operation cannot be carried out because no mirroring hardware is present or available. ",true);
		case DDERR_NONONLOCALVIDMEM :
			TheApp->ErrorBox(szTitle, "An attempt was made to allocate non-local video memory from a device that does not support non-local video memory. ",true);
		case DDERR_NOOPTIMIZEHW :
			TheApp->ErrorBox(szTitle, "The device does not support optimized surfaces. ",true);
		case DDERR_NOOVERLAYDEST :
			TheApp->ErrorBox(szTitle, "The IDirectDrawSurface4::GetOverlayPosition method is called on an overlay that the IDirectDrawSurface4::UpdateOverlay method has not been called on to establish a destination. ",true);
		case DDERR_NOOVERLAYHW :
			TheApp->ErrorBox(szTitle, "The operation cannot be carried out because no overlay hardware is present or available. ",true);
		case DDERR_NOPALETTEATTACHED :
			TheApp->ErrorBox(szTitle, "No palette object is attached to this surface. ",true);
		case DDERR_NOPALETTEHW :
			TheApp->ErrorBox(szTitle, "There is no hardware support for 16- or 256-color palettes. ",true);
		case DDERR_NORASTEROPHW :
			TheApp->ErrorBox(szTitle, "The operation cannot be carried out because no appropriate raster operation hardware is present or available. ",true);
		case DDERR_NOROTATIONHW :
			TheApp->ErrorBox(szTitle, "The operation cannot be carried out because no rotation hardware is present or available. ",true);
		case DDERR_NOSTRETCHHW :
			TheApp->ErrorBox(szTitle, "The operation cannot be carried out because there is no hardware support for stretching. ",true);
		case DDERR_NOT4BITCOLOR :
			TheApp->ErrorBox(szTitle, "The DirectDrawSurface object is not using a 4-bit color palette and the requested operation requires a 4-bit color palette. ",true);
		case DDERR_NOT4BITCOLORINDEX :
			TheApp->ErrorBox(szTitle, "The DirectDrawSurface object is not using a 4-bit color index palette and the requested operation requires a 4-bit color index palette. ",true);
		case DDERR_NOT8BITCOLOR :
			TheApp->ErrorBox(szTitle, "The DirectDrawSurface object is not using an 8-bit color palette and the requested operation requires an 8-bit color palette. ",true);
		case DDERR_NOTAOVERLAYSURFACE :
			TheApp->ErrorBox(szTitle, "An overlay component is called for a non-overlay surface. ",true);
		case DDERR_NOTEXTUREHW :
			TheApp->ErrorBox(szTitle, "The operation cannot be carried out because no texture-mapping hardware is present or available. ",true);
		case DDERR_NOTFLIPPABLE :
			TheApp->ErrorBox(szTitle, "An attempt has been made to flip a surface that cannot be flipped. ",true);
		case DDERR_NOTFOUND :
			TheApp->ErrorBox(szTitle, "The requested item was not found. ",true);
		case DDERR_NOTINITIALIZED :
			TheApp->ErrorBox(szTitle, "An attempt was made to call an interface method of a DirectDraw object created by CoCreateInstance before the object was initialized. ",true);
		case DDERR_NOTLOADED :
			TheApp->ErrorBox(szTitle, "The surface is an optimized surface, but it has not yet been allocated any memory. ",true);
		case DDERR_NOTLOCKED :
			TheApp->ErrorBox(szTitle, "An attempt is made to unlock a surface that was not locked. ",true);
		case DDERR_NOTPAGELOCKED :
			TheApp->ErrorBox(szTitle, "An attempt is made to page unlock a surface with no outstanding page locks. ",true);
		case DDERR_NOTPALETTIZED :
			TheApp->ErrorBox(szTitle, "The surface being used is not a palette-based surface. ",true);
		case DDERR_NOVSYNCHW :
			TheApp->ErrorBox(szTitle, "The operation cannot be carried out because there is no hardware support for vertical blank synchronized operations. ",true);
		case DDERR_NOZBUFFERHW :
			TheApp->ErrorBox(szTitle, "The operation to create a z-buffer in display memory or to perform a blit using a z-buffer cannot be carried out because there is no hardware support for z-buffers. ",true);
		case DDERR_NOZOVERLAYHW :
			TheApp->ErrorBox(szTitle, "The overlay surfaces cannot be z-layered based on the z-order because the hardware does not support z-ordering of overlays. ",true);
		case DDERR_OUTOFCAPS :
			TheApp->ErrorBox(szTitle, "The hardware needed for the requested operation has already been allocated. ",true);
		case DDERR_OUTOFMEMORY :
			TheApp->ErrorBox(szTitle, "DirectDraw does not have enough memory to perform the operation. ",true);
		case DDERR_OUTOFVIDEOMEMORY :
			TheApp->ErrorBox(szTitle, "DirectDraw does not have enough display memory to perform the operation. ",true);
		case DDERR_OVERLAPPINGRECTS :
			TheApp->ErrorBox(szTitle, "Operation could not be carried out because the source and destination rectangles are on the same surface and overlap each other. ",true);
		case DDERR_OVERLAYCANTCLIP :
			TheApp->ErrorBox(szTitle, "The hardware does not support clipped overlays. ",true);
		case DDERR_OVERLAYCOLORKEYONLYONEACTIVE :
			TheApp->ErrorBox(szTitle, "An attempt was made to have more than one color key active on an overlay. ",true);
		case DDERR_OVERLAYNOTVISIBLE :
			TheApp->ErrorBox(szTitle, "The IDirectDrawSurface4::GetOverlayPosition method is called on a hidden overlay. ",true);
		case DDERR_PALETTEBUSY :
			TheApp->ErrorBox(szTitle, "Access to this palette is refused because the palette is locked by another thread. ",true);
		case DDERR_PRIMARYSURFACEALREADYEXISTS :
			TheApp->ErrorBox(szTitle, "This process has already created a primary surface. ",true);
		case DDERR_REGIONTOOSMALL :
			TheApp->ErrorBox(szTitle, "The region passed to the IDirectDrawClipper::GetClipList method is too small. ",true);
		case DDERR_SURFACEALREADYATTACHED :
			TheApp->ErrorBox(szTitle, "An attempt was made to attach a surface to another surface to which it is already attached. ",true);
		case DDERR_SURFACEALREADYDEPENDENT :
			TheApp->ErrorBox(szTitle, "An attempt was made to make a surface a dependency of another surface to which it is already dependent. ",true);
		case DDERR_SURFACEBUSY :
			TheApp->ErrorBox(szTitle, "Access to the surface is refused because the surface is locked by another thread. ",true);
		case DDERR_SURFACEISOBSCURED :
			TheApp->ErrorBox(szTitle, "Access to the surface is refused because the surface is obscured. ",true);
		case DDERR_SURFACELOST :
			TheApp->ErrorBox(szTitle, "Access to the surface is refused because the surface memory is gone. Call the IDirectDrawSurface4::Restore method on this surface to restore the memory associated with it. ",true);
		case DDERR_SURFACENOTATTACHED :
			TheApp->ErrorBox(szTitle, "The requested surface is not attached. ",true);
		case DDERR_TOOBIGHEIGHT :
			TheApp->ErrorBox(szTitle, "The height requested by DirectDraw is too large. ",true);
		case DDERR_TOOBIGSIZE :
			TheApp->ErrorBox(szTitle, "The size requested by DirectDraw is too large. However, the individual height and width are valid sizes. ",true);
		case DDERR_TOOBIGWIDTH :
			TheApp->ErrorBox(szTitle, "The width requested by DirectDraw is too large. ",true);
		case DDERR_UNSUPPORTED :
			TheApp->ErrorBox(szTitle, "The operation is not supported. ",true);
		case DDERR_UNSUPPORTEDFORMAT :
			TheApp->ErrorBox(szTitle, "The FourCC format requested is not supported by DirectDraw. ",true);
		case DDERR_UNSUPPORTEDMASK :
			TheApp->ErrorBox(szTitle, "The bitmask in the pixel format requested is not supported by DirectDraw. ",true);
		case DDERR_UNSUPPORTEDMODE :
			TheApp->ErrorBox(szTitle, "The display is currently in an unsupported mode. ",true);
		case DDERR_VERTICALBLANKINPROGRESS :
			TheApp->ErrorBox(szTitle, "A vertical blank is in progress. ",true);
		case DDERR_VIDEONOTACTIVE :
			TheApp->ErrorBox(szTitle, "The video port is not active. ",true);
		case DDERR_WASSTILLDRAWING :
			TheApp->ErrorBox(szTitle, "The previous blit operation that is transferring information to or from this surface is incomplete. ",true);
		case DDERR_WRONGMODE :
			TheApp->ErrorBox(szTitle, "This surface cannot be restored because it was created in a different mode. ",true);
		case DDERR_XALIGN :
			TheApp->ErrorBox(szTitle, "The provided rectangle was not horizontally aligned on a required boundary. ",true);
		default:
			TheApp->ErrorBox(szTitle, "Unknown Error ",true);
	}

#endif
}