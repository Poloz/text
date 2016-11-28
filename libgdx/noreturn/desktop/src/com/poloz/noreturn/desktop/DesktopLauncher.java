package com.poloz.noreturn.desktop;

import com.badlogic.gdx.backends.lwjgl.LwjglApplication;
import com.badlogic.gdx.backends.lwjgl.LwjglApplicationConfiguration;
import com.poloz.noreturn.NoReturnGame;

public class DesktopLauncher {
	public static void main (String[] arg) {
		LwjglApplicationConfiguration config = new LwjglApplicationConfiguration();
		config.title = NoReturnGame.title;
		config.height = NoReturnGame.heigth;
		config.width = NoReturnGame.width;
		new LwjglApplication(new NoReturnGame(), config);
	}
}
