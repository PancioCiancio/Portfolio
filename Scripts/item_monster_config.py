
import unreal

# 1. SETUP YOUR PATHS HERE
string_table_id = "/Game/Inventory/StringTables/ST_Monsters.ST_Monsters"
texture_folder_path = "/Game/Inventory/Textures/Monsters" # Folder where T_AC_00 etc. live

# Get currently selected assets in the Content Browser
selected_assets = unreal.EditorUtilityLibrary.get_selected_assets()

with unreal.ScopedSlowTask(len(selected_assets), "Updating Data Assets") as slow_task:
    slow_task.make_dialog(True)
    
    for asset in selected_assets:
        if slow_task.should_cancel():
            break
            
        slow_task.enter_progress_frame(1)
        asset_name = asset.get_name() 
        
        # 2. EXTRACT THE NUMBER (Assumes Data Asset is named something like DA_AC_00)
        try:
            # Splits "DA_AC_00" at the underscores and takes the last part "00"
            index_str = asset_name.split('_')[-1] 
            index_int = int(index_str) # Converts "00" to 0
        except ValueError:
            unreal.log_warning(f"Could not parse index from {asset_name}. Skipping.")
            continue

        # 3. BUILD STRING TABLE KEYS
        name_key = f"AC.{index_int}.Name"
        desc_key = f"AC.{index_int}.Desc"

        # 4. GET FTEXT FROM STRING TABLE
        name_text = unreal.TextLibrary.text_from_string_table(string_table_id, name_key)
        desc_text = unreal.TextLibrary.text_from_string_table(string_table_id, desc_key)

        # 5. FIND TEXTURE
        texture_path = f"{texture_folder_path}/T_AC_{index_str}.T_AC_{index_str}"
        texture = unreal.EditorAssetLibrary.load_asset(texture_path)

        # 6. SET PROPERTIES
        # NOTE: Your C++/BP variables must be named exactly "Name", "Description", and "Icon"
        asset.set_editor_property("Name", name_text)
        asset.set_editor_property("Description", desc_text)
        
        if texture:
            asset.set_editor_property("Icon", texture)
        else:
            unreal.log_warning(f"Texture not found for {asset_name} at {texture_path}")

        # Save the asset
        unreal.EditorAssetLibrary.save_loaded_asset(asset)

unreal.log("Batch update complete!")