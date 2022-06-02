<?php
session_start();
?>
<div class="table-responsive">
	<table class="table">
		<thead>
			<tr>
				<th>Nombre</th>
				<th>Ubicacion</th>
				<th>UID</th>
				<th>Día</th>
				<th>Modo</th>
				<th>Config</th>
			</tr>
		</thead>
		<tbody>
			<?php
			require 'connectDB.php';
			$sql = "SELECT * FROM devices ORDER BY id DESC";
			$result = mysqli_stmt_init($conn);
			if (!mysqli_stmt_prepare($result, $sql)) {
				echo '<p class="error">SQL Error</p>';
			} else {
				mysqli_stmt_execute($result);
				$resultl = mysqli_stmt_get_result($result);
				echo '<form action="" method="POST" enctype="multipart/form-data">';
				while ($row = mysqli_fetch_assoc($resultl)) {

					$radio1 = ($row["device_mode"] == 0) ? "checked" : "";
					$radio2 = ($row["device_mode"] == 1) ? "checked" : "";
					$radio3 = ($row["device_mode"] == 2) ? "checked" : "";

					$de_mode = '<div class="mode_select">
					      	<input type="radio" id="' . $row["id"] . '-one" name="' . $row["id"] . '" class="mode_sel" data-id="' . $row["id"] . '" value="0" ' . $radio1 . '/>
					                    <label for="' . $row["id"] . '-one">Registro</label>
		                    <input type="radio" id="' . $row["id"] . '-two" name="' . $row["id"] . '" class="mode_sel" data-id="' . $row["id"] . '" value="1" ' . $radio2 . '/>
					                    <label for="' . $row["id"] . '-two">Asistencia</label>
					                   
							<input type="radio" id="' . $row["id"] . '-tree" name="' . $row["id"] . '" class="mode_sel" data-id="' . $row["id"] . '" value="2" ' . $radio3 . '/>
					                    <label for="' . $row["id"] . '-tree">Control Acceso</label>
					                    </div>';

					echo '<tr>
							        <td>' . $row["device_name"] . '</td>
							        <td>' . $row["device_dep"] . '</td>
							        <td><button type="button" class="dev_uid_up btn btn-warning" id="del_' . $row["id"] . '" data-id="' . $row["id"] . '" title="Actualizar Token"><span class="glyphicon glyphicon-refresh"> </span></button>
							        	' . $row["device_uid"] . '
							        </td>
							        <td>' . $row["device_date"] . '</td>
							        <td>' . $de_mode . '</td>
							        <td>
								    	<button type="button" class="dev_del btn btn-danger" id="del_' . $row["id"] . '" data-id="' . $row["id"] . '" title="Borrar dispositivo"><span class="glyphicon glyphicon-trash"></span></button>
								    </td>
							      </tr>';
				}
				echo '</form>';
			}
			?>
		</tbody>
	</table>
</div>
<!-- <button type="button" class="dev_pro_up btn btn-info" id="del_'.$row["id"].'" data-id="'.$row["id"].'"  title="Cambiar dispositivo"><span class="glyphicon glyphicon-cog"> </span></button> -->