package megabolao.client;

import megabolao.shared.FieldVerifier;
import com.google.gwt.core.client.EntryPoint;
import com.google.gwt.core.client.GWT;
import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.event.dom.client.KeyCodes;
import com.google.gwt.event.dom.client.KeyUpEvent;
import com.google.gwt.event.dom.client.KeyUpHandler;
import com.google.gwt.user.client.Window;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.DialogBox;
import com.google.gwt.user.client.ui.HTML;
import com.google.gwt.user.client.ui.HorizontalPanel;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.RootPanel;
import com.google.gwt.user.client.ui.TextBox;
import com.google.gwt.user.client.ui.ToggleButton;
import com.google.gwt.user.client.ui.VerticalPanel;

/**
 * Entry point classes define <code>onModuleLoad()</code>.
 */
public class MegaBolao implements EntryPoint {
	
	private HorizontalPanel[] hpanel = new HorizontalPanel[5];
	private ToggleButton[] numberButtons = new ToggleButton[50];
	private Label choseButtons = new Label("Escolha 6 numeros: ");
	private Button sendButton = new Button("Enviar");

	/**
	 * This is the entry point method.
	 */
	public void onModuleLoad()
	{
		for( int i = 0; i < hpanel.length; ++i )
		{
			hpanel[i] = new HorizontalPanel();
			hpanel[i].setSpacing(10);
		
			for( int j = 0; j < 10; ++j ) {
				
				int index = (i * 10) + j;
				
				String label = new Integer(index + 1).toString();
				String markedLabel = "[" + label + "]";
				numberButtons[index] = new ToggleButton(label, markedLabel, new ClickHandler() {
					
					@Override
					public void onClick(ClickEvent event) {
						
						String label= "Escolha 6 numeros: ";
						int buttonCount = 0;
						
						for( int i = 0; i < numberButtons.length; ++i )
						{
							if( numberButtons[i].isDown() )
							{
								label += numberButtons[i].getUpFace().getText() + " ";
								buttonCount++;
							}
						}
						
						if( buttonCount > 6 )
							label += " (muitos numeros!)";
						
						choseButtons.setText(label);
					}
				});

				numberButtons[index].setPixelSize(50, 50);
				
				// We can add style names to widgets
				numberButtons[index].addStyleName("numberButton");
		
				// Add the nameField and sendButton to the RootPanel
				// Use RootPanel.get() to get the entire body element
				//RootPanel.get("megabolaoMain").add(numberButtons[i]);
				hpanel[i].add(numberButtons[index]);
				
				RootPanel.get("megabolaoMain").add(hpanel[i]);
			}
		}
		
		sendButton.addClickHandler(new ClickHandler() {
			
			@Override
			public void onClick(ClickEvent event) {
				int numberCount = 0;
				
				for( int i = 0; i < numberButtons.length; ++i )
				{
					if( numberButtons[i].isDown() )
					{
						numberCount++;
					}
				}
				
				if( numberCount == 6 )
					Window.alert("Numeros enviados com sucesso!");
				else
					Window.alert("Voce deve escolher 6 numeros.");
			}
		});
		
		RootPanel.get("megabolaoMain").add(choseButtons);
		RootPanel.get("megabolaoMain").add(sendButton);
	}
}
