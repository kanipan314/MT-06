#include <Novice.h>
#include <math.h>

const char kWindowTitle[] = "学籍番号";


static const int kRowHeigth = 20;
static const int kColumWidth = 60;

struct Vector3
{
	float x;
	float y;
	float z;
};

struct Matrix4x4 {
	float m[4][4];
};

float cot(float x) {
	return 1.0f / tanf(x);
}

void MatrixScreenPrint(int x, int y, const Matrix4x4& matrix, const char* label) {

	Novice::ScreenPrintf(x, y, "%s", label);
	for (int row = 0; row < 4; row++) {
		for (int colum = 0; colum < 4; colum++) {
			Novice::ScreenPrintf(x + colum * kColumWidth, y + (row + 1) * kRowHeigth, "%6.02f", matrix.m[row][colum]);
		}
	}
}

//1.透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovV, float aspectRatio, float nearClip, float farClip) {

	Matrix4x4 answer;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {

			answer.m[i][j] = 0;
		}
	}

	answer.m[0][0] = (1 / aspectRatio) * cot(fovV / 2);
	answer.m[1][1] = cot(fovV / 2);
	answer.m[2][2] = farClip / (farClip - nearClip);
	answer.m[2][3] = 1;
	answer.m[3][2] = (-1 * (nearClip * farClip)) / (farClip - nearClip);

	return answer;
};

//2.正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {

	Matrix4x4 answer;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {

			answer.m[i][j] = 0;
		}
	}

	answer.m[0][0] = 2 / (right - left);
	answer.m[1][1] = 2 / (top - bottom);
	answer.m[2][2] = 1 / (farClip - nearClip);
	answer.m[3][0] = (left + right) / (left - right);
	answer.m[3][1] = (top + bottom) / (bottom - top);
	answer.m[3][2] = nearClip / (nearClip - farClip);
	answer.m[3][3] = 1;

	return answer;

};

//3.ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left,float top, float width, float heigth, float minDepth, float maxDepth) {

	Matrix4x4 answer;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {

			answer.m[i][j] = 0;
		}
	}

	answer.m[0][0] = width / 2;
	answer.m[1][1] = -heigth / 2;
	answer.m[2][2] = maxDepth - minDepth;
	answer.m[3][0] = left + (width / 2);
	answer.m[3][1] = top + (heigth / 2);
	answer.m[3][2] = minDepth;
	answer.m[3][3] = 1;

	return answer;

};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		Matrix4x4 orthographicMatrix = MakeOrthographicMatrix(-160.0f, 160.0f, 200.0f, 300.0f, 0.0f, 1000.0f);

		Matrix4x4 perspectiveFovMatrix = MakePerspectiveFovMatrix(0.63f, 1.33f, 0.1f, 1000.0f);

		Matrix4x4 viewportMatrix = MakeViewportMatrix(100.0f, 200.0f, 600.0f, 300.0f, 0.0f, 1.0f);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		MatrixScreenPrint(0, 0, orthographicMatrix, "orthographicMatrix");
		MatrixScreenPrint(0, kRowHeigth * 5, perspectiveFovMatrix, "perspectiveFovMatrix");
		MatrixScreenPrint(0, kRowHeigth * 10, viewportMatrix, "viewportMatrix");

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
