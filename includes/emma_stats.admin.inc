<?php

function emma_stats_list() {
  $output='';
 $rows =$mailings= array();
  $mailings=emma_get_api_object()->get_mailing_list('true','m,t','s,c','');
  $i=1;
  //echo "<pre>".print_r(($mailings))."</pre>";
 
 if($mailings)
 {
      foreach($mailings as $mailing)
      {
        if($mailing->send_started)
        {
            if($mailing->send_finished)
            {
                $time_date=str_replace('@D:','',$mailing->send_started);
                //list($date,$time)=explode('T',$time_date);
                $time_date=str_replace('T',' ', $time_date);
                $send_details= date('l jS \of F Y h:i:s A', strtotime($time_date));
                //echo $date.'<br>';
                //echo $time.'<br>';
            }
            else
            {
                $send_details="In Progress";
            }
        }
        switch($mailing->mailing_status)
        {
            case "p":$mailing_status="Pending";break;
            case "a":$mailing_status="Paused"; break;
            case "s":$mailing_status="In Progress"; break;
            case "x":$mailing_status="Canceled"; break;
            case "c":$mailing_status="Complete" ;break;
            case "f":$mailing_status="Failed"; break;
            default:$mailing_status="--"; break;
        }
        
        switch($mailing->mailing_type)
        {
            case "m":$mailing_type="Regular"; break;
            case "t":$mailing_type="Test"; break;
            case "r":$mailing_type="Trigger"; break;
            default:$mailing_type="--"; break;
        }    
        $rows[] = array(
          $i,  
          $mailing->name,
          $mailing->subject,
          $mailing_status,
          $mailing_type,
          $mailing->recipient_count,
          $send_details,
          l(t('View Details'), 'admin/config/services/emma_stats/' . $mailing->mailing_id . '/details'),
        );  
        $i++; 
     }  
 } 
 if($rows)
 {
      $table = array(
        'header' => array(t('No:'),t('Name'), t('Mailing Subject'), t('Status'), t('Type'),t('Recipients'), t('Send'), t('Actions')),
        'rows' => $rows
      );
      $output.= theme('table', $table);
  }
  else
  {
      $output.='<div> No Details Found</div><br>';  
  }
  return $output;    
//echo "<pre>".print_r((emma_get_api_object()->get_mailing_list('true','m,t','p,a,s,x,c,f','')),true)."</pre>";
}

function emma_mailing_details($mailing_id = NULL) {
    $emma_api_object=emma_get_api_object();
    $response=$emma_api_object->get_response_overview($mailing_id );
    //echo "<pre>".print_r($response)."</pre>";
    $output='';
    $output.="<h5>Details</h5>";
    $rows[]= array(
      "Total Opens:&nbsp;&nbsp;&nbsp;&nbsp;".(($response->opened)?l(t($response->opened),'admin/config/services/emma_stats/' . $mailing_id . '/details/opens'):'0'),
    );    
    $rows[]= array(
      "Total Clicks:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;".(($response->clicked)?l(t($response->clicked),'admin/config/services/emma_stats/' . $mailing_id . '/details/clicks'):'0'),
    ); 
    $rows[]= array(
      "Total Shares:&nbsp;&nbsp;&nbsp;&nbsp;".(($response->shared)?l(t($response->shared),'admin/config/services/emma_stats/' . $mailing_id . '/details/shares'):'0'),
    ); 
    $rows[]= array(
      "Total Optouts:&nbsp;&nbsp;&nbsp;".(($response->opted_out)?l(t($response->opted_out),'admin/config/services/emma_stats/' . $mailing_id . '/details/optouts'):'0'),
    );  
    $rows[]= array(
      "Total SignUps:&nbsp;&nbsp;&nbsp;".(($response->signed_up)?l(t($response->signed_up),'admin/config/services/emma_stats/' . $mailing_id . '/details/signups'):'0'),
    );     
    $table = array(
    'rows' => $rows
    );
    $output.=theme('table', $table);    
    $output.="<h5>The send-off</h5>";
    $rows2[]= array(
      "Total Emails Sent:&nbsp;&nbsp;&nbsp;&nbsp;".$response->sent,
    );    
    $rows2[]= array(
      "Total Emails Bounced&nbsp;&nbsp;".$response->bounced,
    ); 
    $rows2[]= array(
      "Total Emails received&nbsp;&nbsp;".$response->recipient_count,
    ); 
    $rows2[]= array(
      "Total Emails forwarded&nbsp;&nbsp;".$response->forwarded,
    );    
    
    $table = array(
    'rows' => $rows2
    );
    $output.=theme('table', $table);  
    
    return $output;    
}

function emma_mailing_specific_details($mailing_id = NULL,$action=NULL) {
    $emma_api_object=emma_get_api_object();
    $function="get_".$action;
    $response=$emma_api_object->$function($mailing_id);
    $output='';
   // echo "<pre>".print_r(($response),true)."</pre>";
    $rows=array();
    switch($action)
    {   
        case "signups":
        case "optouts":
        case "opens":
                    $output.="<h5> ".ucfirst($action). " Report </h5> ";
                    $i=1;
                    foreach($response as $info)
                    {
                        $timestamp=str_replace('@D:','',$info->timestamp);
                        $timestamp=str_replace('T',' ', $timestamp);
                        $timestamp= date('l jS \of F Y h:i:s A', strtotime($timestamp));   
                        $user_details=$emma_api_object->get_member_detail($info->member_id);
                        $rows[] = array(
                          $i,  
                          ((isset($user_details->fields->first_name))?$user_details->fields->first_name:'')." ".((isset($user_details->fields->last_name))?$user_details->fields->last_name:''),
                          $info->email,
                          $timestamp,
                        );  
                        $i++;                            
                    }
                    $table = array(
                    'header' => array(t('No:'),t('Name'), t('Email'), t('When')),
                    'rows' => $rows
                    );
                    $output.= theme('table', $table);
                    return $output; 
                    break;   
        case "clicks":
                    $output.="<h5> Links Details </h5> ";
                    $links=$emma_api_object->get_links($mailing_id);
                    $links_row=array();
                    foreach($links as $link)
                    {
                        $i=1;
                        $links_row[$link->link_id] = array(
                          $i,  
                          'link_name'=>$link->link_name ,
                          'link_target'=>$link->link_target,
                          $link->unique_clicks,
                          $link->total_clicks,
                        );  
                        $i++;                            
                    }
                    $table = array(
                    'header' => array(t('No:'),t('Name'), t('Target'), t('Unique Clicks'),t('Total Clicks')),
                    'rows' => $links_row
                    );   
                    $output.= theme('table', $table);   
                    
                    $output.="<h5> Clicks Report </h5> ";                    
                    $i=1;
                    foreach($response as $info)
                    {
                        $timestamp=str_replace('@D:','',$info->timestamp);
                        $timestamp=str_replace('T',' ', $timestamp);
                        $timestamp= date('l jS \of F Y h:i:s A', strtotime($timestamp));   
                        $user_details=$emma_api_object->get_member_detail($info->member_id);
                        $rows[] = array(
                          $i,  
                          ((isset($user_details->fields->first_name))?$user_details->fields->first_name:'')." ".((isset($user_details->fields->last_name))?$user_details->fields->last_name:''),
                          $info->email,
                          $timestamp,
                          $links_row[$info->link_id]['link_name'],
                          $links_row[$info->link_id]['link_target'],
                        );  
                        $i++;                            
                    }
                    $table = array(
                    'header' => array(t('No:'),t('Name'), t('Email'), t('When'),t('Url Name'),t('Url Target')),
                    'rows' => $rows
                    );
                    $output.= theme('table', $table);
                    return $output; 
                    break;

        case "shares":
                    $output.="<h5> ".ucfirst($action). " Report </h5> ";
                    $i=1;
                    foreach($response as $info)
                    {
                        $timestamp=str_replace('@D:','',$info->timestamp);
                        $timestamp=str_replace('T',' ', $timestamp);
                        $timestamp= date('l jS \of F Y h:i:s A', strtotime($timestamp));   
                        $user_details=$emma_api_object->get_member_detail($info->member_id);
                        $rows[] = array(
                          $i,  
                          ((isset($user_details->fields->first_name))?$user_details->fields->first_name:'')." ".((isset($user_details->fields->last_name))?$user_details->fields->last_name:''),
                          $info->email,
                          $timestamp,
                          ucfirst($info->network),
                          $info->share_clicks,
                        );  
                        $i++;                            
                    }
                    $table = array(
                    'header' => array(t('No:'),t('Name'), t('Email'), t('When'),t('Network'),t('Visits from Network')),
                    'rows' => $rows
                    );
                    $output.= theme('table', $table);
                    return $output; 
                    break;                       
                     
    }
}