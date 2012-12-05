<?php

/**
 * @file
 * Emma module admin settings.
 */

/**
 * Return the Emma global settings form.
 */
 
 
function emma_list_table() {
    
  $output='<div>'.l(t('Add New group'), 'admin/config/services/emma_list/groups/add').'</div><br>';

  $rows =$groups= array();
  $emma_api_object=emma_get_api_object();
  $groups=  ($emma_api_object->list_groups('g,t'));
  $i=1;
  $form['submit'] = array(
              '#type' => 'submit',
                      '#value' => t('Save'),
                             '#submit' => array('emma_group_update'));
  if($groups)
  {
      foreach($groups as $group)
      {
        $actions = array(
          l(t('Rename'), 'admin/config/services/emma_list/groups/' . $group->member_group_id  . '/rename'),
          l(t('Delete'), 'admin/config/services/emma_list/groups/' . $group->member_group_id  . '/delete'),
          variable_get('default_group') != $group->member_group_id ? l(t('Set as default'),'admin/config/services/emma_list/set_default_group/' . $group->member_group_id ) : ""
        
         );  
        $rows[] = array(
          $i,  
          l($group->group_name, 'admin/config/services/emma_list/groups/'.$group->member_group_id.'/view'),
          $group->active_count+$group->optout_count+$group->error_count,
          $group->active_count,
          $group->optout_count,
          ($group->group_type=='g')?'Regular':'Test',      
          implode(' | ', $actions),
        );  
        $i++; 
     }    
 }
  if($rows)
  {  
      $table = array(
        'header' => array(t('No:'),t('Name'), t('Total Users'), t('Active'), t('Opt out'),t('Type'), t('Actions')),
        'rows' => $rows
      );
      $output.= theme('table', $table);
  }
  else
  {
    $output.='<div> No Groups Found</div><br>';
  }
  return $output;
}

 

 /* 
 Emma list with table form 
 function emma_list_admin_settings() {

$output='<div>'.l(t('Add New group'), 'admin/config/services/emma_list/groups/add').'</div><br>';
echo $output;
  $groups=  (emma_get_api_object()->list_groups());
  $rows = array();
  $i=1;
  foreach($groups as $group)
  {
    $actions = array(
      l(t('Rename'), 'admin/config/services/emma_list/groups/' . $group->member_group_id  . '/rename'),
      l(t('Delete'), 'admin/config/services/emma_list/groups/' . $group->member_group_id  . '/delete')
    );  
    echo print_r($group,true);
    $rows[$group->member_group_id] = array(
      $i,  
      l($group->group_name, 'admin/config/services/emma_list/groups/'.$group->member_group_id.'/view'),
      $group->active_count+$group->optout_count+$group->error_count,
      $group->active_count,
      $group->optout_count,
      implode(' | ', $actions),
    );  
    $i++; 
 }     
$form['group_table'] = array
(
  '#type' => 'tableselect',
  '#header' => array(t('No:'),t('Name'), t('Total Users'), t('Active'), t('Opt out'), t('Actions')),
  '#options' => $rows,
  '#multiple' => TRUE,
); 

$form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete Selected'),
    '#submit' => array('emma_group_bulk_delete'),
    '#attributes' => array('onclick' => 'if(!confirm("Are you sure delete selected items?")){return false;}'),

);
return $form;

}*/

/*function emma_group_bulk_delete($form, &$form_values){
      //emma_get_api_object()->update_group($form_values['values']['emma_group_id'], $form_values['values']['emma_group_name']);
      //echo "<pre>".print_r(($form_values['values']['group_table']),true)."</pre>";
      foreach($form_values['values']['group_table'] as $val)
      {
            if($val)
                $group_ids[]=$val;
      }
      drupal_set_message(t(print_r(($form_values['values']['group_table']),true)));
    }*/
    
    
function emma_add_group_form() {
 /*
table with form
 $mailings=emma_get_api_object()->get_mailing_list('true','m,t','p,a,s,x,c,f','');
  $rows = array();
  $i=1;
  //echo "<pre>".print_r(($mailings))."</pre>";
  foreach($mailings as $mailing)
  {
    $actions = array(
      l(t('Rename'), 'admin/config/services/emma_list/groups/' . 1 . '/rename'),
      l(t('Delete'), 'admin/config/services/emma_list/groups/' . 1  . '/delete')
    );  
    $rows[] = array(
      $i,  
      $mailing->name,
      $mailing->mailing_status,
      $mailing->mailing_type,
      implode(' | ', $actions),
    );  
    $i++; 
 }     
 $header=array(t('No:'),t('Name'), t('Status'), t('Type'),  t('Actions'));
$form['table'] = array
(
  '#type' => 'tableselect',
  '#header' => $header,
  '#options' => $rows,
  '#multiple' => TRUE,
); */
  $form['emma_group_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Group Name'),
      '#required' => TRUE,
      '#default_value' => '',
  );  
  $form['emma_group_type'] = array(
      '#type' => 'checkbox',
      '#title' => t('Make this a test group?'),
       '#default_value' => 0,
  );  
        $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
       '#submit' => array('emma_group_add'),
     
      );
       return $form;
}

function emma_group_add($form, &$form_values){
      $group = array( 'groups' => array( array(
                                            'group_name' => $form_values['values']['emma_group_name'],
                                            'group_type' => ($form_values['values']['emma_group_type'])?'t':'g'
                                            ) ) );
      $response=emma_get_api_object()->create_groups($group);
      if(isset($response[0]->member_group_id))
        drupal_set_message(t("New Group Created"));
      else
        drupal_set_message(t("Ooops Action failed"),'error');
    }

function emma_set_default_group($group = NULL){
#    $group_details=  (emma_get_api_object()->get_group_detail($group));
 #   if(isset($group_details)){
     variable_set('default_group',$group);
  #  }
   # else
    #    drupal_set_message(t("No group found"));
   drupal_goto($path = 'admin/config/services/emma_list');
}
 
function emma_list_list_form($form, &$form_state, $group = NULL) {

  $group_details=  (emma_get_api_object()->get_group_detail($group));
  $form['emma_group_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Group Name'),
      '#required' => TRUE,
      '#default_value' => ($group_details->group_name)?$group_details->group_name:'',
  );  
  $form['emma_group_id'] = array(
      '#type' => 'hidden',
      '#title' => t('Group Id'),
      '#required' => TRUE,
      '#default_value' => $group,
  );    
        $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
       '#submit' => array('emma_group_update'),
     
      );
       return $form;
}

function emma_group_update($form, &$form_values){
      emma_get_api_object()->update_group($form_values['values']['emma_group_id'], $form_values['values']['emma_group_name']);
      drupal_set_message(t('Update Successfull !!!!'));
    }
    
function emma_delete_list_form($form, &$form_state, $group = NULL) {
    $group_details=  (emma_get_api_object()->get_group_detail($group));
    if(isset($group_details->member_group_id))
    {
        $form_state['group'] = $group;
          $form['emma_group_id'] = array(
          '#type' => 'hidden',
          '#title' => t('Group Id'),
          '#required' => TRUE,
          '#default_value' => $group,
      );   
        return confirm_form($form,
        t('Are you sure you want to delete %name ?', array('%name' => $group_details->group_name)),
        'admin/config/services/emma_list',
        t('This action cannot be undone'),
        t('Delete Group'));
    }
    else
    {
        drupal_goto($path = 'admin/config/services/emma_list');
    }    
}

function emma_delete_list_form_submit($form, &$form_values){
      if($form_values['values']['confirm'])
      {
        $response=  (emma_get_api_object()->delete_group($form_values['group']));
        if($response)
        {
            drupal_set_message(t('Delete Successfull !!!!'));      
        }    
      }
}

function emma_view_group_details($form, &$form_state,$group = NULL) {

//var_dump(entity_load('user'));
  $members=  emma_get_api_object()->list_group_members($group);
  $rows = array();
  $i=1;
  //echo "<pre>".print_r(emma_get_api_object(),true)."</pre>";
    $output='<div>'.l(t('Add Members to group'), 'admin/config/services/emma_list/groups/'.$group.'/add_user').'</div><br>';
  foreach($members as $member)
  {
    $time_date=str_replace('@D:','',$member->member_since);
    //list($date,$time)=explode('T',$time_date);
    $time_date=str_replace('T',' ', $time_date);
    $member_since= date('l jS \of F Y h:i:s A', strtotime($time_date));  
    $actions = array(
      l(t('Edit Status'), 'admin/config/services/emma_list/groups/member/'.$member->member_id.'/edit_status'),
      l(t('Edit Details'), 'admin/config/services/emma_list/groups/'.$group.'/member/'.$member->member_id.'/edit_details'),
      l(t('Delete'), 'admin/config/services/emma_list/groups/'.$group.'/member/'.$member->member_id.'/delete'),
    );  
    $rows[$member->member_id] = array(
      $i,  
      ((isset($member->fields->first_name))?$member->fields->first_name:'')." ".((isset($member->fields->last_name))?$member->fields->last_name:''),
      $member->email,
      $member_since,
      ucfirst($member->status),
      implode(' | ', $actions),
    );  
    $i++; 
 }     
 
 //var_dump($rows);
    if($rows)
    {
         $form['html'] = array(
          '#prefix' => $output,
        );
        $form['table_users'] = array
        (
          '#type' => 'tableselect',
          '#header' => array(t('No:'),t('Name'), t('Email'),t('Member Since'), t('Status'), t('Actions')),
          '#options' => $rows,
          '#multiple' => TRUE,
        ); 

        $form['submit_delete'] = array(
            '#type' => 'submit',
            '#value' => t('Delete Selected'),
            '#submit' => array('emma_delete_user_form_submit'),
            '#attributes' => array('onclick' => 'if(!confirm("Are you sure delete selected Users?")){return false;}'),

        );

        /*$form['submit_copy'] = array(
            '#type' => 'submit',
            '#value' => t('Copy To groups'),
            '#submit' => array('emma_copy_users_form'),
            '#attributes' => array('onclick' => 'if(!confirm("Are you sure delete selected Users?")){return false;}'),

        );*/

    } 
    else
    {
        $output.='<div> No Members Found</div><br>';
         $form['html'] = array(
          '#prefix' => $output,
        );        
    }
 


//$form['#action']=url('admin/config/services/emma_list/groups/member/copy_users');
 /* $table = array(
    'header' => array(t('No:'),t('Name'), t('Total Users'), t('Active'), t('Opt out'), t('Actions')),
    'rows' => $rows
  );
 // $output.= theme('table', $table);*/
  return $form;
}

/* function emma_copy_users_form($form, &$form_state)
{

    lists_session("type", $form_state['input']['table_users']);
    $groups=emma_get_api_object()->list_groups();   
    foreach($groups as $group)
    {
        $groups_list[$group->member_group_id]=$group->group_name;
    }
    $form['group_list'] = array(
      '#type' => 'select',
      '#title' => t('Select Groups'),
      '#default_value' => '',
      '#multiple' => TRUE ,
      '#size' => count($groups_list)>10 ? 10:count($groups_list),
      '#options' => $groups_list,
    );    
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#submit' => array('emma_copy_users_form_submit'),
  );
  return $form;
}

function emma_copy_users_form_submit()
{
    drupal_set_message(print_r(lists_session("type"),true));
}

*/
function emma_add_user_form($form, &$form_state, $group = NULL) {
    //echo "<pre>".print_r((emma_get_api_object()->get_member_detail(90347832)),true)."</pre>";
    $form_state['group'] = $group;
    $emma_api_object=emma_get_api_object();
    $fields=$emma_api_object->get_field_list(1);
    $i=0;
    foreach($fields as $field)
    {  
        $options=array();
        if($field->options)
        {
            foreach($field->options as $key=>$val)
            {
                $options[$val]=$val;
            }
        }
        if($field->widget_type=="text")
        {
            $form[$field->shortcut_name] = array(
              '#type' => 'textfield',
              '#title' => t($field->display_name),
              '#default_value' => '',
             // '#required' => TRUE,
            );              
        }    
        else if($field->widget_type=="radio")
        {
            $form[$field->shortcut_name] = array(
              '#type' => 'radios',
              '#title' => t($field->display_name),
              '#options' => $options,
              //'#default_value' => reset($options),
            );              
        }   
        else if($field->widget_type=="check_multiple")
        {
            $form[$field->shortcut_name] = array(
              '#type' => 'checkboxes',
              '#title' => t($field->display_name),
              '#options' => $options,
              //'#default_value' => array(reset($options)),
            );              
        }    
        else if($field->widget_type=="select multiple")
        {
            $form[$field->shortcut_name] = array(
              '#type' => 'select',
              '#title' => t($field->display_name),
              //'#default_value' => reset($options),
              '#multiple' => TRUE ,
              '#size' => 5,
              '#options' => $options,
            );              
        }   
        else if($field->widget_type=="long")
        {
            $form[$field->shortcut_name] = array(
              '#type' => 'textarea',
              '#title' => t($field->display_name),
              '#default_value' => '',
            );              
        }        
        else if($field->widget_type=="date")
        {
            $form[$field->shortcut_name] = array(
              '#type' => 'date',
              '#title' => t($field->display_name),
              '#default_value' => array('year' => 'x', 'month' => 'x', 'day' => 'x'),
              '#element_validate'=>array('date_custom_validate'),
              '#pre_render' => array('add_null_date'),
            );              
        }            
        $i++;
    }
        $form['emma_email'] = array(
          '#type' => 'textfield',
          '#title' => t('Email'),
          '#required' => TRUE,
          '#default_value' => '',
        );       
      /*  $form['emma_preferred'] = array(
          '#type' => 'select',
          '#title' => t('Preferred format'),
          '#default_value' => '',
          '#multiple' => false,
          '#options' => array('False' => t('HTML'), 'True' => t('Plaintext')), 
        );           
        */
        $groups=$emma_api_object->list_groups('g,t');   
        foreach($groups as $group)
        {
            $groups_list[$group->member_group_id]=$group->group_name;
        }
        $form['group_list'] = array(
          '#type' => 'select',
          '#title' => t('Add to Groups'),
          '#default_value' => $form_state['group'],
          '#multiple' => TRUE ,
          '#size' => count($groups_list)>10 ? 10:count($groups_list),
          '#options' => $groups_list,
        );            
        $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
       '#submit' => array('emma_user_add'),
     
      );
       return $form;    
}

    function emma_add_user_form_validate($form, &$form_state) {
        if(!valid_email_address($form_state['values']['emma_email']))
            form_set_error('', t('Please enter a valid email.'));
        $response=(emma_get_api_object()->get_member_detail_by_email($form_state['values']['emma_email']));
        if(isset($response->member_id))
        {
            //form_set_error('', t('Sorry User exists.'));
            drupal_set_message('That member already exists in your audience. You can update the information below and save your changes. ','error');
            drupal_goto($path = 'admin/config/services/emma_list/groups/'.$response->member_id.'/member/'.$response->member_id.'/edit_details');
        }
    }
    function emma_user_add($form, &$form_values){
    //$fields = array('first_name'=>'Foo', 'last_name'=>'Bar');
    //$groups = '270648';
    //$response = $E->import_single_member('test1@gmail.com', $fields, $groups, $signup_form);  
    $emma_api_object=emma_get_api_object();
    $fields=$emma_api_object->get_field_list(1);
    $field_data=array();
    $form_values_data=array();
    foreach($fields as $field)
    {  
        if(1)
        {
            if($field->widget_type=='check_multiple'||$field->widget_type=="select multiple")
            {
                //drupal_set_message(print_r($form_values['values'][$field->shortcut_name],true));
                if($form_values['values'][$field->shortcut_name] )
                {
                    foreach($form_values['values'][$field->shortcut_name] as $key=>$val)
                    {
                        if($val)
                            $form_values_data['values'][$field->shortcut_name][]=$val;
                    }
                }
                else
                {   
                    $form_values_data['values'][$field->shortcut_name]=array();
                }
                if(isset($form_values_data['values'][$field->shortcut_name]) && $form_values_data['values'][$field->shortcut_name])
                {
                    $field_data[$field->shortcut_name]=$form_values_data['values'][$field->shortcut_name];
                }
            }
            else if($field->widget_type=="date")
            {
                if($form_values['values'][$field->shortcut_name]&& $form_values['values'][$field->shortcut_name]['year']!='x' && $form_values['values'][$field->shortcut_name]['day']!='x' && $form_values['values'][$field->shortcut_name]['year'] !='x')
                {
                    $field_data[$field->shortcut_name]='@D:'.$form_values['values'][$field->shortcut_name]['year'].'-'.$form_values['values'][$field->shortcut_name]['month'].'-'.$form_values['values'][$field->shortcut_name]['day'];
                }                
            }
            else
            {
                $field_data[$field->shortcut_name]=$form_values['values'][$field->shortcut_name];
            }    
        }    
    }
        $groups=array();
      if(isset($form_values['values']['group_list']))
      {
        foreach($form_values['values']['group_list'] as $val)
        {
            if($val)
            {
                $groups[]=intval($val);
            }
        }
      }
      $response=$emma_api_object->import_single_member($form_values['values']['emma_email'], $field_data, $groups);
      if(isset($response->member_id))
      {
        drupal_set_message(t('New Users Added Successfully')); 
        /*$group_response=$emma_api_object->add_member_to_groups($response->member_id,$form_values['values']['group_list']);
        $diff=array_diff($form_values['values']['group_list'], $group_response);
        if(!$group_response||$diff)
        {
            drupal_set_message(t('There are too many members in your test group already or there was an problem adding this member to one of the selected groups. Please check your selection and try again. '),'error');
        }*/
      }   
      else
      {
            drupal_set_message(t('Ooops, Something went wrong'),'error'); 
      }
     
     //drupal_set_message(print_r($form_values['values']['group_list'],true));
    }
    
    function emma_edit_user_form($form, &$form_state, $member = NULL) {
        $emma_api_object=emma_get_api_object();
        $form_state['member']=$member;
        $member_details=($emma_api_object->get_member_detail($member));
        $user_status=substr($member_details->status, 0, 1);
        $form_state['user_status']=$user_status;
        $form['emma_user_status'] = array(
          '#type' => 'select',
          '#title' => t('Status'),
          '#default_value' => substr($member_details->status, 0, 1),
          '#multiple' => false,
          '#options' => array('a' => t('Active'), 'e' => t('Error'),'o' => t('Opt-Out')), 
        );           
        $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
       '#submit' => array('emma_edit_user_submit'),
     
      );
       return $form;  
    }
    
    
    function emma_edit_user_submit($form, &$form_values)
    {
        $emma_api_object=emma_get_api_object();
        $member_ids = array($form_values['member']);
        if($form_values['values']['emma_user_status']!=$form_values['user_status'])
        {
        $response=$emma_api_object->update_members_status($member_ids, $form_values['values']['emma_user_status']);
        if(!isset($response->error) && $response==1)
            drupal_set_message(t('Status Changed Successfully !!!'));
        else
            drupal_set_message(t('Sorry, Users who have opted out of your list are uneditable. !!'),'error');
        }
        else
        {
            drupal_set_message(t('Status Changed Successfully !!!'));
        }
    }
    function emma_delete_user_form($form, &$form_state, $group = NULL,$member = NULL) {
        $member_details=  (emma_get_api_object()->get_member_detail($member));
        if(isset($member_details->member_id))
        {
            $first_name=isset($member_details->fields->first_name)?$member_details->fields->first_name:'';
            $last_name=isset($member_details->fields->last_name)?$member_details->fields->last_name:'';
            
            $form_state['member'] = $member;
            return confirm_form($form,
            t('Are you sure you want to delete %name ?', array('%name' => $first_name." ".$last_name )),
            'admin/config/services/emma_list/groups/'.$group.'/view',
            t('This will delete the user from all the groups & action cannot be undone'),
            t('Delete'));
        }
        else
        {
            drupal_goto($path = 'admin/config/services/emma_list/groups/'.$group.'/view');
        }    
    }

    function emma_delete_user_form_submit($form, &$form_values){
            $member_ids=array();
            if(isset($form_values['member']) && $form_values['member']!='*')
            {
                $member_ids = array($form_values['member']);
            }
            else
            {
                foreach($form_values['input']['table_users'] as $val)
                {
                    if($val)
                    {
                        $member_ids[]=$val;
                    }    
                    
                }           
            }    
            if($member_ids)
            {
                $response = emma_get_api_object()->delete_members($member_ids);      
                if(isset($response->member_ids)||$response==1)
                {
                    drupal_set_message(t('Delete Successfull !!!!'));      
                }
            }
            else
            {
                drupal_set_message(t('Please select a member'),'error');      
            }
    }

function emma_edit_details_user_form($form, &$form_state, $group = NULL,$member=null) {
    //echo "<pre>".print_r((emma_get_api_object()->get_member_detail(90347832)),true)."</pre>";
    $form_state['group'] = $group;
    $form_state['member'] = $member;
    $emma_api_object=emma_get_api_object();
    $fields=$emma_api_object->get_field_list(1);
    $member_details=  ($emma_api_object->get_member_detail($member));
    $form_state['member_status'] =substr($member_details->status, 0, 1);
   echo "<pre>".print_r($member_details,true)."</pre>";
    $groups=  ($emma_api_object->list_member_groups($member));
    $member_group_ids=array();
    foreach($groups as $group)
    {
        $member_group_ids[]=$group->member_group_id;
    }
    $i=0;
    foreach($fields as $field)
    {  
        $options=array();
        if($field->options)
        {
            foreach($field->options as $key=>$val)
            {
                $options[$val]=$val;
            }
        }
        $fields="fields";
        $shortcut_name=$field->shortcut_name;
        if($field->widget_type=="text")
        {
            $form[$field->shortcut_name] = array(
              '#type' => 'textfield',
              '#title' => t($field->display_name),
             '#default_value' => (isset($member_details->$fields->$shortcut_name)&& $member_details->$fields->$shortcut_name)?$member_details->$fields->$shortcut_name:'',
             // '#required' => TRUE,
            );              
        }    
        else if($field->widget_type=="radio")
        {
            $form[$field->shortcut_name] = array(
              '#type' => 'radios',
              '#title' => t($field->display_name),
              '#options' => $options,
              '#default_value' => (isset($member_details->$fields->$shortcut_name)&& $member_details->$fields->$shortcut_name)?$member_details->$fields->$shortcut_name:'',
            );              
        }   
        else if($field->widget_type=="check_multiple")
        {
            $form[$field->shortcut_name] = array(
              '#type' => 'checkboxes',
              '#title' => t($field->display_name),
              '#options' => $options,
             // '#default_value' => ''
            );   
            if((isset($member_details->$fields->$shortcut_name)&& $member_details->$fields->$shortcut_name))
            {
                $form[$field->shortcut_name]['#default_value']=$member_details->$fields->$shortcut_name;
            }
        }    
        else if($field->widget_type=="select multiple")
        {
            $form[$field->shortcut_name] = array(
              '#type' => 'select',
              '#title' => t($field->display_name),
              '#default_value' => (isset($member_details->$fields->$shortcut_name)&& $member_details->$fields->$shortcut_name)?$member_details->$fields->$shortcut_name:'',
              '#multiple' => TRUE ,
              '#size' => 5,
              '#options' => $options,
            );              
        }   
        else if($field->widget_type=="long")
        {

            $form[$field->shortcut_name] = array(
              '#type' => 'textarea',
              '#title' => t($field->display_name),
              '#default_value' => (isset($member_details->$fields->$shortcut_name)&& $member_details->$fields->$shortcut_name)?$member_details->$fields->$shortcut_name:'',
            );              
        }        
        else if($field->widget_type=="date")
        {
           // @D:1900-02-18 
           $form_state['dates_fields'][]=$field->shortcut_name;
            if((isset($member_details->$fields->$shortcut_name)&& $member_details->$fields->$shortcut_name))
            {
                $date=str_replace('@D:','',$member_details->$fields->$shortcut_name);
                list($year,$month,$day)=explode('-',$date);
                $form[$field->shortcut_name] = array(
                  '#type' => 'date',
                  '#title' => t($field->display_name),
                  '#default_value' => array('year' => $year, 'month' => $month, 'day' => $day),
                  '#element_validate'=>array('date_custom_validate'),
                );   
            }            
            else
            {
                $form[$field->shortcut_name] = array(
                  '#type' => 'date',
                  '#title' => t($field->display_name),
                  '#default_value' => array('year' => 'x', 'month' => 'x', 'day' => 'x'),
                  '#element_validate'=>array('date_custom_validate'),
                  '#pre_render' => array('add_null_date'),
                );                   
            }
        }            
        $i++;
    }
    
       $form['emma_email'] = array(
          '#type' => 'textfield',
          '#title' => t('Email'),
          '#required' => TRUE,
          '#default_value' => $member_details->email?$member_details->email:'',
        );
        //$form_state['emma_email_id']=$member_details->email?$member_details->email:'';
        /*$form['emma_preferred'] = array(
          '#type' => 'select',
          '#title' => t('Preferred format'),
          '#default_value' => $member_details->plaintext_preferred,
          '#multiple' => false,
          '#options' => array('0' => t('HTML'), '1' => t('Plaintext')), 
        );  */         
        
        $groups=$emma_api_object->list_groups('g,t');   
        foreach($groups as $group)
        {
            $groups_list[$group->member_group_id]=$group->group_name;
        }
        $form['group_list'] = array(
          '#type' => 'select',
          '#title' => t('Add to Groups'),
          '#default_value' => $member_group_ids,
          '#multiple' => TRUE ,
          '#size' => count($groups_list)>10 ? 10:count($groups_list),
          '#options' => $groups_list,
        );            
        $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
       '#submit' => array('emma_user_update'),
     
      );
       return $form;    
}

    function emma_edit_details_user_form_validate($form, &$form_state) {
        if(!valid_email_address($form_state['values']['emma_email']))
            form_set_error('', t('Please enter a valid email.'));
    }    

    function emma_user_update($form, &$form_values){
    //$fields = array('first_name'=>'Foo', 'last_name'=>'Bar');
    //$groups = '270648';
    //$response = $E->import_single_member('test1@gmail.com', $fields, $groups, $signup_form);  
    $emma_api_object=emma_get_api_object();
    $fields=$emma_api_object->get_field_list(1);
    $field_data=array();
    $form_values_data=array();
    foreach($fields as $field)
    {  
        if(1)
        {
            if($field->widget_type=='check_multiple'||$field->widget_type=="select multiple")
            {
                //drupal_set_message(print_r($form_values['values'][$field->shortcut_name],true));
                if($form_values['values'][$field->shortcut_name] )
                {
                    foreach($form_values['values'][$field->shortcut_name] as $key=>$val)
                    {
                        if($val)
                            $form_values_data['values'][$field->shortcut_name][]=$val;
                    }
                }
                else
                {   
                    //drupal_set_message(print_r($form_values['values'][$field->shortcut_name],true));
                    $form_values_data['values'][$field->shortcut_name]=array();
                }
                if(isset($form_values_data['values'][$field->shortcut_name]) && $form_values_data['values'][$field->shortcut_name])
                {
                    $field_data[$field->shortcut_name]=$form_values_data['values'][$field->shortcut_name];
                }
            }
            else if($field->widget_type=="date")
            {
                if($form_values['values'][$field->shortcut_name]&& $form_values['values'][$field->shortcut_name]['year']!='x' && $form_values['values'][$field->shortcut_name]['day']!='x' && $form_values['values'][$field->shortcut_name]['year'] !='x')
                {
                   echo $field_data[$field->shortcut_name]='@D:'.$form_values['values'][$field->shortcut_name]['year'].'-'.$form_values['values'][$field->shortcut_name]['month'].'-'.$form_values['values'][$field->shortcut_name]['day'];
                    
                }                
            }
            else
            {
                $field_data[$field->shortcut_name]=$form_values['values'][$field->shortcut_name];
            }    
        }    
    }
     $groups=array();
      if(isset($form_values['values']['group_list']))
      {
        foreach($form_values['values']['group_list'] as $val)
        {
            if($val)
            {
                $groups[]=intval($val);
            }
        }
      }    
      $emma_api_object->remove_member_from_all_groups($form_values['member']);
      
      $response=$emma_api_object->update_member($form_values['member'],$form_values['values']['emma_email'], $form_values['member_status'],  $field_data);

      if($response==1 && !(isset($response->error)))
      {
        //$emma_api_object->remove_member_from_all_groups($response->member_id);
        $update_response=$emma_api_object->add_member_to_groups($form_values['member'],$groups);        
        drupal_set_message(t('Update Successfull !!'));
      }   
      else
      {
            drupal_set_message(t('Oooops ..Some thing went wrong'),'error'); 
      }
      //drupal_set_message(print_r($form_values['emma_email_id'],true));
    }
    
/*function emma_import_users_form($form, &$form_state,$group = NULL){
    $form_state['group']=$group;
    $emma_api_object=emma_get_api_object();
    $groups=$emma_api_object->list_groups('g,t');   
    foreach($groups as $group)
    {
        $groups_list[$group->member_group_id]=$group->group_name;
    }
    $form['#attributes']['enctype'] = 'multipart/form-data'; // If this is not here, upload will fail on submit
    $form['fid'] = array( '#title' => t('Upload a File'),
    '#type' => 'file',
    '#description' => t('Files must be in Excel (xls or xlsx) or csv format.'),
    );
    //echo $group;
    $form['group_list'] = array(
      '#type' => 'select',
      '#title' => t('Add to Groups'),
      '#default_value' => array($form_state['group']),
      '#multiple' => TRUE ,
      '#size' => count($groups_list)>10 ? 10:count($groups_list),
      '#options' => $groups_list,
    );        
    $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('emma_import_users_form_submit'),
    );
    return $form;
}    
*/

/*function emma_import_users_form_submit($form, &$form_values){

    drupal_set_message(print_r($form_values['values'],true));
    $filepath = 'public://temp/';
    file_prepare_directory($filepath, FILE_CREATE_DIRECTORY);
    $file = file_save_upload('fid', array('file_validate_extensions' => array('xls xlsx csv')), $filepath,FILE_EXISTS_RENAME);
    // Very important to do this or file will be deleted on cron
   // $file->status = FILE_STATUS_PERMANENT;
    $file = file_save($file);
    $members=array('email' =>'sdasdasdas@ding.com');
    if($file){
        
        $filename=$GLOBALS['base_url']."/".variable_get('file_public_path', conf_path() . '/files').'/temp/'.$file->filename;
        $emma_api_object=emma_get_api_object();
        $response=$emma_api_object->import_member_list($members, $filename);      
        drupal_set_message(print_r($response,true));
 
    }elseif(!$file){
    drupal_set_message('There was an error uploading the file', $type = 'error');
    } 
 
}*/


function lists_session($key, $value = NULL) {
  static $storage;
  if ($value) {
    $storage[$key] = $value ;
    $_SESSION['lists'][$key] = $value ;   // I put 'lists' in case some other module uses 'type' in $_SESSION
  }
  else if (empty($storage[$key]) && isset($_SESSION['lists'][$key])) {
    $storage[$key] = $_SESSION['lists'][$key];
  }
  return $storage[$key];
}
    
function add_null_date($form) {
  $form['year']['#options']['x']='--';
  $form['month']['#options']['x']='--';
  $form['day']['#options']['x']='--';
  return $form;
}    
    
function date_custom_validate($element) {
     if(!$element['#value']['day']||!$element['#value']['month']||!$element['#value']['day'])
     {
        if(!(!$element['#value']['day']&&!$element['#value']['month']&&!$element['#value']['day']))
        {
            form_error($element, t('The specified date is invalid.'));
        }
     }
    

}    
