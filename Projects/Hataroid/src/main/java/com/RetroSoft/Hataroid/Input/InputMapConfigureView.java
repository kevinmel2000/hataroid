package com.RetroSoft.Hataroid.Input;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import android.app.AlertDialog;
import android.app.ListActivity;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.Bundle;
import android.preference.PreferenceManager;
//import android.support.v7.app.ActionBar;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ListView;
import android.widget.Spinner;
import android.widget.TextView;

import com.RetroSoft.Hataroid.HataroidActivity;
import com.RetroSoft.Hataroid.R;
//import com.RetroSoft.Hataroid.Util.AppCompatListActivity;

//public class InputMapConfigureView extends AppCompatListActivity implements OnItemSelectedListener
public class InputMapConfigureView extends ListActivity implements OnItemSelectedListener
{
	final static int INPUTCAPTURERESULT_KEYCODE		= 1;
	final static int RENAMEINPUTMAPRESULT_KEYCODE	= 2;
	
	public static final String	kPrefPrefix = "_PREFInputMap_";
	public final static String	kPrefLastPresetIDKey = kPrefPrefix + "Settings_LastPresetID";
	public final static String	kPrefUserPresetPrefix = kPrefPrefix + "Settings_UserPresetMap_";
	final static String			kPrefUserPresetOrder = kPrefPrefix + "Settings_UserPresetOrder";

	InputMapArrayAdapter	_adapter;
	Intent					_retIntent;
	
	List<String>			_presetIDList = new LinkedList<String>();
	List<String>			_presetNameList = new LinkedList<String>();

	Map<String,InputMap>	_userPresetList = new HashMap<String,InputMap>();
	List<String>			_userPresetOrder = new LinkedList<String>();
	Map<String,String>		_userPresetNameList = new HashMap<String,String>();

	int						_curPresetIdx = -1;
	String					_curPresetID = null;
	InputMap				_curInputMap = null;

	int                     _localeID = Input.kLocale_EN;

	@Override public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.configureinputmap_view);
		
		_parseOptions(savedInstanceState);

		_localeID = Input.kLocale_EN;
		try {
			if (HataroidActivity.instance != null) {
				_localeID = HataroidActivity.instance.getInput().getLocaleID();
			}
		} catch (Exception e) {
			e.printStackTrace();
		} catch (Error e) {
			e.printStackTrace();
		}

//		try
//		{
//			ActionBar actionBar = getSupportActionBar();
//
//			LayoutInflater mInflater = LayoutInflater.from(this);
//			View customView = mInflater.inflate(R.layout.presetlist_actionbar, null);
//
//			actionBar.setCustomView(customView);
//			actionBar.setDisplayShowCustomEnabled(true);
//
//			TextView tv = (TextView)customView.findViewById(R.id.ab_title);
//			tv.setText(getApplicationContext().getString(R.string.ab_title_inputmap_configure));
//		}
//		catch (Exception e)
//		{
//			e.printStackTrace();
//		}

		try
		{
			View mView = this.findViewById(R.id.inputmapconfigure_view);

			TextView title = (TextView)mView.findViewById(R.id.ab_title);
			title.setText(this.getTitle());

			ImageButton navBackBtn = (ImageButton)mView.findViewById(R.id.nav_back);
			navBackBtn.setOnClickListener(new View.OnClickListener() {
				public void onClick(View view) {
					sendFinish(RESULT_OK);
				}
			});
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}

		getListView().setItemsCanFocus(true);

		_setupButtonListeners();

		_retIntent = new Intent();
		
		_readSavedPrefs();
		
		_updatePresetListUIItem();
		_refreshCurSelection();
	}
	
	void _parseOptions(Bundle savedInstanceState)
	{
		Bundle b = (savedInstanceState == null) ? getIntent().getExtras() : savedInstanceState;
		if (b != null)
		{
		}
	}

	static List<String> _loadUserPresetOrder(SharedPreferences prefs, String ignorePreset)
	{
		List<String> userPresetOrder = new LinkedList<String>();
		String presetOrderStr = prefs.getString(kPrefUserPresetOrder, null);
		if (presetOrderStr != null)
		{
			String [] data = presetOrderStr.split(",");
			if (data != null && data.length > 0)
			{
				for (int i = 0; i < data.length; ++i)
				{
					if (ignorePreset == null || ignorePreset.compareTo(data[i]) != 0)
					{
						userPresetOrder.add(data[i]);
					}
				}
			}
		}
		return userPresetOrder;
	}

	void _readSavedPrefs()
	{
		SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(getApplicationContext());
		
		// order
		{
			_userPresetOrder = _loadUserPresetOrder(prefs, null);
		}
		
		List<String> invalidEntries = new LinkedList<String>();

		// user input maps
		{
			_userPresetList.clear();
			_userPresetNameList.clear();
			
	    	Map<String,?> allPrefs = prefs.getAll();
			for (Map.Entry<String,?> entry : allPrefs.entrySet())
			{
				String key = entry.getKey();
				if (key.startsWith(kPrefUserPresetPrefix))
				{
					boolean error = false;
					String id = key.replace(kPrefUserPresetPrefix, "");
					if (!_userPresetOrder.contains(id))
					{
						if (!invalidEntries.contains(key))
						{
							invalidEntries.add(key);
						}
						continue;
					}
					
					try
					{
						Map<String,Object> result = new HashMap<String,Object>();
						error = !Input.decodeInputMapPref(entry.getValue().toString(), _localeID, result);
						if (!error)
						{
							_userPresetNameList.put(id, (String)result.get("name"));
							_userPresetList.put(id, (InputMap)result.get("map"));
						}
					}
					catch (Exception e)
					{
						e.printStackTrace();
						error = true;
					}
					
					if (error)
					{
						if (!invalidEntries.contains(key))
						{
							invalidEntries.add(key);
						}
					}
				}
			}
		}

		// remove invalid entries
		{
			for (int i = 0; i < _userPresetOrder.size(); ++i)
			{
				String id = _userPresetOrder.get(i);
				if (!_userPresetList.containsKey(id))
				{
					String key = kPrefUserPresetPrefix + id;
					if (!invalidEntries.contains(key))
					{
						invalidEntries.add(key);
					}
					_userPresetOrder.remove(i);
					--i;
				}
			}
			if (invalidEntries.size() > 0)
			{
				Editor ed = prefs.edit();
				for (int i = 0; i < invalidEntries.size(); ++i)
				{
					ed.remove(invalidEntries.get(i));
				}
				ed.commit();
			}
		}

		// cur selected map
		{
			_setupPresetList();

			_curPresetIdx = 0;
			_curPresetID = _presetIDList.get(_curPresetIdx);

			String lastPresetID = prefs.getString(kPrefLastPresetIDKey, null);
			if (lastPresetID != null)
			{
				int idx = _presetIDList.indexOf(lastPresetID);
				if (idx >= 0)
				{
					_curPresetIdx = idx;
					_curPresetID = lastPresetID;
				}
			}
		}
	}
	
	void _setupPresetList()
	{
		_presetIDList.clear();
		_presetNameList.clear();
		
		for (int i = 0; i < Input.kPreset_NumOf; ++i)
		{
			_presetIDList.add(Input.getPresetID(i));
			_presetNameList.add(Input.getPresetName(i) + " (read only)");
		}
		
		for (int i = 0; i < _userPresetOrder.size(); ++i)
		{
			String id = _userPresetOrder.get(i);
			_presetIDList.add(id);
			_presetNameList.add(_userPresetNameList.get(id));
		}
	}

	@Override protected void onDestroy()
	{
		super.onDestroy();
	}

	@Override protected void onPause()
	{
		super.onPause();
	}
	
	@Override protected void onResume()
	{
		super.onResume();
	}

	@Override public boolean onKeyDown(int keyCode, KeyEvent event)
	{
		switch (keyCode)
		{
			case KeyEvent.KEYCODE_BACK:
			{
				sendFinish(RESULT_CANCELED);
			}
		}

		return super.onKeyDown(keyCode, event);
	}

//	@Override
//	public boolean onOptionsItemSelected(MenuItem item)
//	{
//		switch (item.getItemId())
//		{
//			case android.R.id.home:
//			{
//				sendFinish(RESULT_OK);
//				return true;
//			}
//		}
//
//		return super.onOptionsItemSelected(item);
//	}

	void _setupButtonListeners()
	{
//		findViewById(R.id.im_closeBtn).setOnClickListener(new OnClickListener() {
//			public void onClick(View v) {
//				sendFinish(RESULT_OK);
//			}
//		});

//		ActionBar actionBar = null;
//		View actionBarView = null;
//		try
//		{
//			actionBar = getSupportActionBar();
//			if (actionBar != null)
//			{
//				actionBarView = actionBar.getCustomView();
//			}
//		}
//		catch (Exception e)
//		{
//			e.printStackTrace();
//		}
		View actionBarView = this.findViewById(R.id.inputmapconfigure_view);


		try
		{
			if (actionBarView != null)
			{
				View spinner = findViewById(R.id.im_presetSpinner);
				spinner.setNextFocusUpId(R.id.im_showKeyboardBtn);

				View vb = null;

				vb = actionBarView.findViewById(R.id.im_deleteBtn);
				vb.setNextFocusDownId(R.id.im_presetSpinner);
				vb.setOnClickListener(new OnClickListener() {
					public void onClick(View v) {
						_onDeleteClicked();
					}
				});

				vb = actionBarView.findViewById(R.id.im_renameBtn);
				vb.setNextFocusDownId(R.id.im_presetSpinner);
				vb.setOnClickListener(new OnClickListener() {
					public void onClick(View v) {
						_onRenameClicked();
					}
				});

				vb = actionBarView.findViewById(R.id.im_newBtn);
				vb.setNextFocusDownId(R.id.im_presetSpinner);
				vb.setOnClickListener(new OnClickListener() {
					public void onClick(View v) {
						_onNewClicked();
					}
				});

				vb = actionBarView.findViewById(R.id.im_showKeyboardBtn);
				vb.setNextFocusDownId(R.id.im_presetSpinner);
				vb.setNextFocusRightId(R.id.im_presetSpinner);
				//vb.setNextFocusForwardId(R.id.im_presetSpinner);
				vb.setOnClickListener(new OnClickListener() {
					public void onClick(View v) {
						// show input method selector
						Input input = HataroidActivity.instance.getInput();
						input.showInputMethodSelector();
					}
				});
			}
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
	}
	
	void _setModifyButtonsEnabled(boolean enabled)
	{
		Button b = (Button)findViewById(R.id.im_deleteBtn);
		if (b != null)
		{
			b.setEnabled(enabled);
		}
		
		b = (Button)findViewById(R.id.im_renameBtn);
		if (b != null)
		{
			b.setEnabled(enabled);
		}
	}

	void _updatePresetListUIItem()
	{
		Spinner spList = (Spinner)findViewById(R.id.im_presetSpinner);
		if (spList != null)
		{
			ArrayAdapter<String> presetListAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_dropdown_item);
			for (int i = 0; i < _presetIDList.size(); ++i)
			{
				presetListAdapter.add(_presetNameList.get(i));
			}

			spList.setAdapter(presetListAdapter);
			spList.setOnItemSelectedListener(this);
		}
	}
	
	void _setupInputMapItems()
	{
		_curInputMap = null;
		if (_curPresetID == null)
		{
			return;
		}

		if (Input.isSystemPreset(_curPresetID))
		{
			// system preset
			int [] presetArray = Input.getPresetArray(Input.getPresetID(_curPresetID));
			if (presetArray != null)
			{
				_curInputMap = new InputMap();
				_curInputMap.initFromArray(Input.kNumSrcKeyCodes, presetArray, _localeID);
			}
		}
		else
		{
			// user preset
			_curInputMap = _userPresetList.get(_curPresetID);
			if (_curInputMap == null)
			{
				// create new one
				_curInputMap = new InputMap();
				_curInputMap.init(Input.kNumSrcKeyCodes, _localeID);
				_userPresetList.put(_curPresetID, _curInputMap);
			}
		}

		if (_curInputMap != null)
		{
			Map<Integer, List<Integer>> emuToSystemMap = _curInputMap.destToSrcMap;
	
			List<InputMapListItem> items = new ArrayList<InputMapListItem>();
			for (int i = 0; i < VirtKeyDef.VKB_KEY_NumOf; ++i)
			{
				VirtKeyDef vkDef = VirtKeyDef.kDefs[i];
				if (vkDef.config > 0)
				{
					List<Integer> systemKeysList = emuToSystemMap.get(vkDef.id);
					int [] systemKeys = null;
					if (systemKeysList != null && systemKeysList.size() > 0)
					{
						systemKeys = new int [systemKeysList.size()];
						for (int s = 0; s < systemKeysList.size(); ++s)
						{
							systemKeys[s] = systemKeysList.get(s);
						}
					}
					items.add(new InputMapListItem(vkDef, systemKeys, _localeID));
				}
			}
			Collections.sort(items);
			
			_adapter = new InputMapArrayAdapter(this, R.layout.configureinputmap_item, items);
			setListAdapter(_adapter);
		}

		_setModifyButtonsEnabled(!Input.isSystemPreset(_curPresetID));
	}

	@Override protected void onListItemClick(ListView l, View v, int position, long id)
	{
		super.onListItemClick(l,  v, position, id);

		InputMapListItem item = _adapter.getItem(position);
		onMapBtnClicked(item);
	}

	public void onMapBtnClicked(InputMapListItem item)
	{
		if (!_verifyCanModifyCurPreset())
		{
			return;
		}

		showGetKeycodeDialog(item);
	}

	public void onUnMapBtnClicked(InputMapListItem item)
	{
		if (!_verifyCanModifyCurPreset())
		{
			return;
		}

		String prevMapId = _curPresetID;
		int prevEmuKey = item._vkDef.id;

		int systemKey = -1;
		boolean unMap = true;

		_updateInputMapKey(prevMapId, prevEmuKey, systemKey, unMap);
	}

	boolean _verifyCanModifyCurPreset()
	{
		if (Input.isSystemPreset(_curPresetID))
		{
			AlertDialog alertDialog = new AlertDialog.Builder(this).create();
			alertDialog.setTitle("Built-in PRESET");
			alertDialog.setMessage("This preset is read only. If you want to change the settings, please click the NEW button below or choose a different preset.");
			alertDialog.setButton(AlertDialog.BUTTON_POSITIVE, "Ok", new DialogInterface.OnClickListener() { public void onClick(DialogInterface dialog, int which) {  } });
			alertDialog.show();
			return false;
		}
		return true;
	}

	public void onItemSelected(AdapterView<?> parent, View view, int pos, long id) {
		if (pos >= 0 && pos < _presetIDList.size())
		{
			_curPresetIdx = pos;
			_curPresetID = _presetIDList.get(_curPresetIdx);
			_setupInputMapItems();
			
			_storeSelectedInputMapID(_curPresetID, getApplicationContext());
		}
    }

    public void onNothingSelected(AdapterView<?> parent) {
    }
    
    private void sendFinish(int resultCode)
	{
		setResult(resultCode, _retIntent);
		finish();
	}

    public void showGetKeycodeDialog(InputMapListItem scanItem)
    {
		final InputMapConfigureView ctx = this;
		final int resultID = INPUTCAPTURERESULT_KEYCODE;

        Intent view = new Intent(ctx, InputCaptureView.class);
        view.putExtra(InputCaptureView.CONFIG_EMUKEY, scanItem._vkDef.id);
        //view.putExtra(InputCaptureView.CONFIG_SYSTEMKEY, scanItem._systemKey);
	    view.putExtra(InputCaptureView.CONFIG_CANCANCEL, true);
        view.putExtra(InputCaptureView.CONFIG_MAPID, _curPresetID);
        ctx.startActivityForResult(view, resultID);
	}
    
    public void showRenameDialog()
    {
    	if (_curPresetIdx < 0 || _curPresetIdx >= _presetIDList.size())
    	{
    		return;
    	}
    	String curName = _presetNameList.get(_curPresetIdx);
    	if (curName == null)
    	{
    		return;
    	}
    	
		final InputMapConfigureView ctx = this;
		final int resultID = RENAMEINPUTMAPRESULT_KEYCODE;

        Intent view = new Intent(ctx, RenameInputMapView.class);
        view.putExtra(RenameInputMapView.CONFIG_CURNAME, curName);
        ctx.startActivityForResult(view, resultID);
    }
    
	@Override protected void onActivityResult(int requestCode, int resultCode, Intent data)
	{
		switch (requestCode)
		{
			case INPUTCAPTURERESULT_KEYCODE:
			{
				if (resultCode == RESULT_OK)
				{
					int prevEmuKey = data.getIntExtra(InputCaptureView.RESULT_PREVEMUKEY, -1);
					//int prevSystemKey = data.getIntExtra(InputCaptureView.RESULT_PREVSYSTEMKEY, -1);
					String prevMapId = data.getStringExtra(InputCaptureView.RESULT_PREVMAPID);

					boolean unMap = data.getBooleanExtra(InputCaptureView.RESULT_UNMAP, false);
					int systemKey = data.getIntExtra(InputCaptureView.RESULT_KEYCODE, -1);

					_updateInputMapKey(prevMapId, prevEmuKey, systemKey, unMap);
				}
				break;
			}
			case RENAMEINPUTMAPRESULT_KEYCODE:
			{
				if (resultCode == RESULT_OK)
				{
					String newName = data.getStringExtra(RenameInputMapView.RESULT_NAME);
					if (newName != null)
					{
						newName = newName.trim();
						newName = newName.replaceAll(",", ".");
					}
					if (newName != null && newName.length() > 0 && _curPresetIdx >= 0 && _curPresetIdx < _presetIDList.size())
					{
						int presetIdx = _curPresetIdx;
						
						String curName = _presetNameList.get(_curPresetIdx);
						if (curName.compareTo(newName) != 0)
						{
							_presetNameList.set(_curPresetIdx, newName);
							_userPresetNameList.put(_curPresetID, newName);
							_updatePresetListUIItem();

							Spinner spList = (Spinner)findViewById(R.id.im_presetSpinner);
							if (spList != null)
							{
								spList.setSelection(presetIdx);
							}
						}

						_storeInputMap(_curPresetID);
					}
				}
				break;
			}
		}
	}

	boolean _updateInputMapKey(String prevMapId, int prevEmuKey, int systemKey, boolean unMap)
	{
		if (_curInputMap == null || _adapter == null)
		{
			return false;
		}

		if (prevEmuKey < 0  || prevMapId == null || prevMapId.compareTo(_curPresetID) != 0)
		{
			return false;
		}

		if (unMap)
		{
			//_curInputMap.removeKeyMapEntry(prevSystemKey);
			_curInputMap.removeDestKeyMapEntry(prevEmuKey);
		}
		else if (systemKey >= 0)
		{
			_curInputMap.addKeyMapEntry(systemKey, prevEmuKey);
		}

		// update list items
		{
			int numItems = _adapter.getCount();
			Map<Integer, List<Integer>> emuToSystemMap = _curInputMap.destToSrcMap;
			if (emuToSystemMap != null)
			{
				for (int i = 0; i < numItems; ++i)
				{
					InputMapListItem li = _adapter.getItem(i);
					if (li != null)
					{
						int [] systemKeys = null;
						List<Integer> systemKeysList = emuToSystemMap.get(li._vkDef.id);
						if (systemKeysList != null && systemKeysList.size() > 0)
						{
							systemKeys = new int [systemKeysList.size()];
							for (int s = 0; s < systemKeysList.size(); ++s)
							{
								systemKeys[s] = systemKeysList.get(s);
							}
						}
						li._systemKeys = systemKeys;
					}
				}
			}
		}

		_adapter.notifyDataSetChanged();

		_storeInputMap(_curPresetID);

		return true;
	}

	boolean _showingDeleteConfirm = false;
	void _onDeleteClicked()
	{
		if (Input.isSystemPreset(_curPresetID))
		{
			return;
		}

		if (!_showingDeleteConfirm)
		{
			_showingDeleteConfirm = true;
			AlertDialog alertDialog = new AlertDialog.Builder(this).create();
			alertDialog.setTitle("Delete Input Map?");
			alertDialog.setMessage("Are you sure you want to delete this input map?");
			alertDialog.setButton(AlertDialog.BUTTON_POSITIVE, "No", new DialogInterface.OnClickListener() { public void onClick(DialogInterface dialog, int which) { _showingDeleteConfirm = false; } });
			alertDialog.setButton(AlertDialog.BUTTON_NEGATIVE, "Yes", new DialogInterface.OnClickListener() { public void onClick(DialogInterface dialog, int which) { _showingDeleteConfirm = false; _tryDeleteInputMap(); } });
			alertDialog.setOnCancelListener(new DialogInterface.OnCancelListener() {
				public void onCancel(DialogInterface dialog) { _showingDeleteConfirm = false; }
			});
			alertDialog.show();
		}
	}
	
	void _tryDeleteInputMap()
	{
		if (_curPresetID == null || Input.isSystemPreset(_curPresetID))
		{
			return;
		}

		int presetPos = _curPresetIdx;
		if (presetPos >= 0)
		{
			int newSelection = -1;

			_presetIDList.remove(presetPos);
			_presetNameList.remove(presetPos);
			
			if (_userPresetList.containsKey(_curPresetID))
			{
				_userPresetList.remove(_curPresetID);
			}
			if (_userPresetNameList.containsKey(_curPresetID))
			{
				_userPresetNameList.remove(_curPresetID);
			}
			int presetOrderIdx = _userPresetOrder.indexOf(_curPresetID);
			if (presetOrderIdx >= 0)
			{
				_userPresetOrder.remove(presetOrderIdx);
			}
			
			_storeItemDeleted(_curPresetID);

			_curPresetID = null;
			newSelection = presetPos - 1;
			if (newSelection < 0 && _presetIDList.size() > 0)
			{
				newSelection = 0;
			}
			
			_curPresetIdx = newSelection;
			_updatePresetListUIItem();
			_refreshCurSelection();
		}
	}
	
	void _onRenameClicked()
	{
		if (Input.isSystemPreset(_curPresetID))
		{
			return;
		}
		
		showRenameDialog();
	}

	void _onNewClicked()
	{
		// get unique name
		final String newNamePrefix = "my_input_map";
		int nextID = 1;
		for (int i = 0; i < _presetNameList.size(); ++i)
		{
			String [] nameSplit = _presetNameList.get(i).split(" ");
			if (nameSplit != null && nameSplit.length == 2 && nameSplit[0].compareTo(newNamePrefix) == 0)
			{
				try
				{
					int curID = Integer.parseInt(nameSplit[1]);
					if (nextID <= curID) { nextID = curID + 1;; }
				}
				catch (Exception e) {}
			}
		}
		
		String newPresetID = java.util.UUID.randomUUID().toString();
		String newPresetName = newNamePrefix + " " + nextID;

		_presetIDList.add(newPresetID);
		_presetNameList.add(newPresetName);
		
		// create new input map
		InputMap map = new InputMap();
		map.init(Input.kNumSrcKeyCodes, _localeID);
		_userPresetList.put(newPresetID, map);
		_userPresetOrder.add(newPresetID);
		_userPresetNameList.put(newPresetID, newPresetName);

		_storeInputMap(newPresetID);

		int newSelection = _presetIDList.size()-1;
		_curPresetIdx = newSelection;
		_updatePresetListUIItem();
		_refreshCurSelection();
	}

	void _refreshCurSelection()
	{
		if (_curPresetIdx >= 0)
		{
			Spinner spList = (Spinner)findViewById(R.id.im_presetSpinner);
			if (spList != null)
			{
				spList.setSelection(_curPresetIdx);
			}
		}
	}

	static void _storeSelectedInputMapID(String id, Context ctx)
	{
		if (id != null)
		{
			SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(ctx);

			Editor ed = prefs.edit();
			ed.putString(kPrefLastPresetIDKey, id);
			ed.commit();
		}
	}

	void _storeInputMap(String id)
	{
		if (id != null && _userPresetList.containsKey(id))
		{
			InputMap map = _userPresetList.get(id);
			String mapName = _userPresetNameList.get(id);

			_writeInputMapToPrefs(id, mapName, map, _userPresetOrder, -1, getApplicationContext());
		}
	}

	static void _writeInputMapToPrefs(String mapID, String mapName, InputMap map, List<String> userPresetOrder, int autoAddPresetOrder, Context ctx)
	{
		SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(ctx);

		if (autoAddPresetOrder >= 0)
		{
			userPresetOrder = _loadUserPresetOrder(prefs, mapID);
			if (autoAddPresetOrder == 0)
			{
				userPresetOrder.add(0, mapID);
			}
			else
			{
				userPresetOrder.add(mapID);
			}
		}

		Editor ed = prefs.edit();

		_updateUserPresetOrderPrefs(prefs, ed, userPresetOrder);

		String itemKey = kPrefUserPresetPrefix + mapID;
		String mapStr = map.encodePrefString(mapName);
		if (mapStr != null)
		{
			ed.putString(itemKey, mapStr);
		}

		ed.commit();
	}

	void _storeItemDeleted(String id)
	{
		if (id != null)
		{
			SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(getApplicationContext());
			
			Editor ed = prefs.edit();

			_updateUserPresetOrderPrefs(prefs, ed, _userPresetOrder);
			
			String itemKey = kPrefUserPresetPrefix + id;
			if (prefs.contains(itemKey))
			{
				ed.remove(itemKey);
			}
			
			ed.commit();
		}
	}
	
	static void _updateUserPresetOrderPrefs(SharedPreferences prefs, Editor ed, List<String> userPresetOrder)
	{
		String order = null;
		if (userPresetOrder.size() > 0)
		{
			order = userPresetOrder.get(0);
			for (int i = 1; i < userPresetOrder.size(); ++i)
			{
				order += "," + userPresetOrder.get(i);
			}
		}

		if (order == null)
		{
			if (prefs.contains(kPrefUserPresetOrder))
			{
				ed.remove(kPrefUserPresetOrder);
			}
		}
		else
		{
			ed.putString(kPrefUserPresetOrder, order);
		}
	}

	public static boolean createTVInputMap(Integer[] assignKeys, Integer[] assignVals, Context ctx, int localeID)
	{
		String id = "TVInputMap";
		String mapName = "Android TV Input Map";

		InputMap map = new InputMap();
		map.init(Input.kNumSrcKeyCodes, localeID);

		int defKeyMap[] = new int[]
		{
			KeyEvent.KEYCODE_DPAD_UP, VirtKeyDef.VKB_KEY_JOYUP,
			KeyEvent.KEYCODE_DPAD_DOWN, VirtKeyDef.VKB_KEY_JOYDOWN,
			KeyEvent.KEYCODE_DPAD_LEFT, VirtKeyDef.VKB_KEY_JOYLEFT,
			KeyEvent.KEYCODE_DPAD_RIGHT, VirtKeyDef.VKB_KEY_JOYRIGHT,
			KeyEvent.KEYCODE_DPAD_CENTER, VirtKeyDef.VKB_KEY_JOYFIRE,
			KeyEvent.KEYCODE_BUTTON_A, VirtKeyDef.VKB_KEY_JOYFIRE,
			KeyEvent.KEYCODE_BUTTON_A, VirtKeyDef.VKB_KEY_MOUSELB,
			KeyEvent.KEYCODE_BUTTON_B, VirtKeyDef.VKB_KEY_MOUSERB,
		};

		for (int i = 0; i < defKeyMap.length; i += 2)
		{
			int systemKey = defKeyMap[i];
			int emuKey = defKeyMap[i+1];
			map.addKeyMapEntry(systemKey, emuKey);
		}

		for (int i = 0; i < assignKeys.length; ++i)
		{
			int systemKey = assignVals[i];
			int emuKey = assignKeys[i];
			map.addKeyMapEntry(systemKey, emuKey);
		}

		_writeInputMapToPrefs(id, mapName, map, null, 0, ctx);

		// select and enable input map
		_storeSelectedInputMapID(id, ctx);
		Input.storeEnableInputMap(true, ctx);

		return true;
	}
}
