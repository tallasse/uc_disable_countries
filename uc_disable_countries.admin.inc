<?php
/**
 * @file
 * Admin page callbacks for the uc_disable_countries module.
 */

/**
 * Form used on admin settings page to select which zones should be disabled.
 *
 * @ingroup forms
 */
function uc_disable_countries_settings_form($form, $form_state, $section = 'delivery') 
{
	$countries = _uc_disable_countries_get_country_list();
	$currently_disabled = variable_get('uc_disable_countries_' . $section);

	$form['text'] = array(
	'#value' => t('Select countries to disable for @section.', array('@section' => $section)),
	);

	/* adding check all code */
	drupal_add_js("
					function checkUncheckAll(theElement){
					  var theForm = theElement.form, z = 0;
					  for(z=0; z<theForm.length;z++){
						if(theForm[z].type == 'checkbox' && theForm[z].name != 'checkall'){
						  theForm[z].checked = theElement.checked;
						}
					  }
					}
				  ", 'inline');
				  
	$form['checkall'] = array(
		'#type' => 'checkbox',
		'#title' => t('Select / Unselect all'),
		'#attributes' => array('onclick' => 'checkUncheckAll(this);'),
		'#weight' => -1,
		);
	/* ending check all code */

	// create fieldset to hold country list, for neatness
	$form['countryDisplay'] = array(
      '#title' => t('Countries'),
      '#type' => 'fieldset',
      '#collapsed' => FALSE,
      '#collapsible' => TRUE,
    );

	// create countryList form to make all options part of array when form is returned
	$form['countryDisplay']['countryList'] = array(
		'#type' => 'checkboxes',
		'#options' => $countries,
		'#default_value' => $currently_disabled,
		);

	$form['section'] = array(
		'#type' => 'hidden',
		'#value' => $section,
		);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Save'),
		);

	return $form;
}

/**
 * Form submission handler for uc_disable_countries_settings_form().
 *
 * @see uc_disable_countries_settings_form()
 */
function uc_disable_countries_settings_form_submit($form, &$form_state) 
{
	$disabled = array();
	
	// cycle through every 'countryList' array index, see if it's checked
	foreach ($form_state['values']['countryList'] as $country_id => $checked) 
	{
		if ($checked) // if it is, add is to the disabled list
		{
			$disabled[$country_id] = $checked;
		}
			
	}

	// if we specified 'billing' vs 'delivery' in the settings, use that, else default to 'delivery'
	if (isset($form_state['values']['section']))
	{
		$section = $form_state['values']['section'];
	}
	else
	{
		$section = 'delivery';
	}
	
	variable_set('uc_disable_countries_' . $section, $disabled);
	drupal_set_message(t('@section settings saved.', array('@section' => $section)));
}
